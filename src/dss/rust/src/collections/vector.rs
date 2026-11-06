#![allow(unused, dead_code)]

pub mod vec {
    use core::ptr::NonNull;
    use core::{marker::PhantomData, slice};

    use core::cmp;
    use std::ops::Deref;
    use std::{
        alloc::{Layout, alloc, dealloc, handle_alloc_error, realloc},
        env::current_exe,
        mem,
        ops::{Bound, RangeBounds},
        ptr,
    };

    /// error for capacity overflow
    fn capacity_overflow() -> ! { panic!("capacity overflow") }

    /// the buffer pointer + capacity, knows how to grow,
    /// and owns the allocations 
    #[rustfmt::skip]
    pub struct RawVec<T> { ptr: NonNull<T>, cap: usize }
    #[rustfmt::skip]
    pub struct Vec<T> { buf: RawVec<T>, len: usize }

    pub struct IntoIter<T> {
        buf: NonNull<T>,
        cap: usize,
        ptr: *const T,
        end: *const T,
        _marker: PhantomData<T>,
    }

    pub struct Drain<'a, T> {
        tail_start: usize,
        tail_len: usize,
        iter: core::slice::Iter<'a, T>,
        vec: NonNull<Vec<T>>,
    }

    unsafe impl<T: Send> Send for RawVec<T> {}
    unsafe impl<T: Sync> Sync for RawVec<T> {}
    unsafe impl<T: Send> Send for IntoIter<T> {}
    unsafe impl<T: Sync> Sync for IntoIter<T> {}
    unsafe impl<T: Send> Send for Drain<'_, T> {}
    unsafe impl<T: Sync> Sync for Drain<'_, T> {}

    impl<T> RawVec<T> {
        const IS_ZST: bool = mem::size_of::<T>() == 0;
        const MIN_NON_ZERO_CAP: usize = if mem::size_of::<T>() == 1 {
            8
        } else if mem::size_of::<T>() <= 1024 {
            4
        } else {
            1
        };

        /// ZSTs never allocate, so they have "infinite" capacity.
        pub const fn new() -> Self {
            RawVec {
                ptr: NonNull::dangling(),
                cap: if Self::IS_ZST {
                    usize::MAX
                } else {
                    0
                },
            }
        }

        pub fn with_capacity(cap: usize) -> Self {
            if Self::IS_ZST || cap == 0 {
                return Self::new();
            }
            let layout = Layout::array::<T>(cap).unwrap_or_else(|_| capacity_overflow());
            let p = unsafe { alloc(layout) } as *mut T;
            match NonNull::new(p) {
                Some(ptr) => RawVec { ptr, cap },
                None => handle_alloc_error(layout),
            }
        }

        pub fn ptr(&self) -> *mut T { self.ptr.as_ptr() }
        pub fn capacity(&self) -> usize { self.cap }

        fn grow_amortized(&mut self, len: usize, additional: usize) {
            /// A ZST alaready has capacity usize::MAX, so getting here
            /// means overflow
            if Self::IS_ZST {
                capacity_overflow()
            }
            let required = len.checked_add(additional).unwrap_or_else(|| capacity_overflow());
            let cap = cmp::max(self.cap * 2, required);
            let cap = cmp::max(Self::MIN_NON_ZERO_CAP, cap);
            self.finish_grow(cap);
        }

        pub fn grow_one(&mut self) { self.grow_amortized(self.cap, 1); }
        pub fn reserve(&mut self, len: usize, additional: usize) {
            /// if len > cap (it can never happen) but this will still triggers
            /// growth to happen
            if self.cap.wrapping_sub(len) < additional {
                self.grow_amortized(len, additional);
            }
        }

        pub fn shrink_to_fit(&mut self, cap: usize) {
            assert!(cap <= self.cap, "Tried to shrink to a larger capacity");
            // if the new capacity is the same as the current capacity theres
            // nothing to shrink, or if is usize::MAX
            if Self::IS_ZST || cap == self.cap {
                return;
            }

            let old_layout = Layout::array::<T>(self.cap).unwrap();
            if cap == 0 {
                unsafe { dealloc(self.ptr.as_ptr() as *mut u8, old_layout) }

                // just making sure the state inside the vec does not remains
                // broken or undefined after dealloc
                self.ptr = NonNull::dangling();
                self.cap = 0;
                return;
            }

            let new_layout = Layout::array::<T>(cap).unwrap();
            let p = unsafe { realloc(self.ptr.as_ptr() as *mut u8, old_layout, new_layout.size()) };
            match NonNull::new(p as *mut T) {
                Some(p) => {
                    self.ptr = p;
                    self.cap = cap;
                }
                None => handle_alloc_error(new_layout),
            };
        }

        // Helper methods
        fn finish_grow(&mut self, new_cap: usize) {
            let new_layout = Layout::array::<T>(new_cap).unwrap_or_else(|_| capacity_overflow());
            let p = unsafe {
                if self.cap == 0 {
                    alloc(new_layout);
                } else {
                    let old_layout = Layout::array::<T>(self.cap).unwrap();
                    realloc(self.ptr.as_ptr() as *mut u8, old_layout, new_layout.size());
                }
            };
        }
    }

    impl<T> Drop for RawVec<T> {
        fn drop(&mut self) {
            if !Self::IS_ZST && self.cap != 0 {
                unsafe {
                    let layout = Layout::array::<T>(self.cap).unwrap();
                    dealloc(self.ptr.as_ptr() as *mut u8, layout);
                }
            }
        }
    }

    impl<T> Vec<T> {
        pub const fn new() -> Self { Vec { buf: RawVec::new(), len: 0 } }
        pub fn with_capacity(capacity: usize) -> Self { Vec { buf: RawVec::with_capacity(capacity), len: 0 } }
        pub unsafe fn from_raw_parts(ptr: *mut T, length: usize, capacity: usize) -> Self {
            Vec {
                buf: RawVec {
                    ptr: unsafe { NonNull::new_unchecked(ptr) },
                    cap: if RawVec::<T>::IS_ZST {
                        usize::MAX
                    } else {
                        capacity
                    },
                },
                len: length,
            }
        }

        pub fn capacity(&self) -> usize { self.buf.capacity() }
        pub fn len(&self) -> usize { self.len }
        pub fn is_empty(&self) -> bool { self.len == 0 }
        pub fn reserve(&mut self, additional: usize) { self.buf.reserve(self.len, additional); }

        pub fn shrink_to_fit(&mut self) {
            if self.capacity() > self.len {
                self.buf.shrink_to_fit(self.len);
            }
        }
        pub fn push(&mut self, value: T) {
            if self.len == self.buf.capacity() {
                self.buf.grow_one();
            }
            unsafe {
                ptr::write(self.buf.ptr().add(self.len), value);
            }
            self.len += 1;
        }
        pub fn pop(&mut self) -> Option<T> {
            if self.len == 0 {
                None
            } else {
                self.len -= 1;
                unsafe { Some(ptr::read(self.buf.ptr().add(self.len))) }
            }
        }

        pub fn insert(&mut self, index: usize, element: T) {
            let len = self.len;
            assert!(index <= len, "insertion index (is {index}) should be <= len (is {len})");
            if len == self.buf.capacity() {
                self.buf.grow_one();
            }
            unsafe {
                let p = self.buf.ptr().add(index);
                ptr::copy(p, p.add(1), len - index);
                ptr::write(p, element);
            }
            self.len = len + 1;
        }

        pub fn remove(&mut self, index: usize) -> T {
            let len = self.len;
            unsafe {
                let p = self.buf.ptr().add(index);
                let value = ptr::read(p);
                ptr::copy(p.add(1), p, len - index - 1);
                self.len = len - 1;
                value
            }
        }

        pub fn swap_remove(&mut self, index: usize) -> T {
            let len = self.len;
            assert!(index < len, "swap_remove index (is {index}) should be < len (is {len})");
            unsafe {
                let base = self.buf.ptr();
                let value = ptr::read(base.add(index));
                ptr::copy(base.add(len - 1), base.add(index), 1);
                self.len = len - 1;
                value
            }
        }

        pub fn truncate(&mut self, len: usize) {
            if len >= self.len {
                return;
            }
            unsafe {
                let remaining = self.len - len;
                let tail = ptr::slice_from_raw_parts_mut(self.buf.ptr().add(len), remaining);
            }
        }

        pub fn clear(&mut self) { self.truncate(0); }
        pub fn append(&mut self, other: &mut Vec<T>) {
            let n = other.len;
            self.reserve(n);
            unsafe {
                ptr::copy_nonoverlapping(other.buf.ptr(), self.buf.ptr(), n);
            }
            self.len += n;
            other.len = 0;
        }

        pub fn split_off(&mut self, at: usize) -> Vec<T> {
            assert!(at <= self.len, "`at` split index (is {at}) should be <= len (is {})", self.len);
            let other_len = self.len - at;
            let mut other = Vec::with_capacity(other_len);
            unsafe {
                self.len = at;
                ptr::copy_nonoverlapping(self.buf.ptr().add(at), other.buf.ptr(), other_len);
            }
            other.len = other_len;
            other
        }

        pub fn resize(&mut self, new_len: usize, value: T)
        where
            T: Clone,
        {
            if new_len > self.len {
                let n = new_len - self.len;
                self.reserve(n);
                for _ in 1..n {
                    self.push(value.clone());
                }
                self.push(value);
            } else {
                self.truncate(new_len);
            }
        }

        pub fn extend_from_slice(&mut self, other: &[T])
        where
            T: Clone,
        {
            self.reserve(other.len());
            for x in other {
                self.push(x.clone());
            }
        }

        pub fn retain<F: FnMut(&T) -> bool>(&mut self, mut f: F) {
            let original_len = self.len;
            if original_len == 0 {
                return;
            }

            // If `f` or a destructor panics we must not expose
            // moved-out/dropped slots.
            self.len = 0;

            struct Guard<'a, T> {
                v: &'a mut Vec<T>,
                processed: usize,
                deleted: usize,
                original_len: usize,
            }
            impl<T> Drop for Guard<'_, T> {
                fn drop(&mut self) {
                    unsafe {
                        if self.deleted > 0 && self.processed < self.original_len {
                            let p = self.v.buf.ptr();
                            ptr::copy(p.add(self.processed), p.add(self.processed - self.deleted), self.original_len - self.processed);
                        }
                    }
                    self.v.len = self.original_len - self.deleted;
                }
            }

            let mut g = Guard { v: self, processed: 0, deleted: 0, original_len };
            while g.processed < original_len {
                unsafe {
                    let current = g.v.buf.ptr().add(g.processed);
                    if !f(&*current) {
                        g.processed += 1;
                        g.deleted += 1;
                        ptr::drop_in_place(current);
                    } else {
                        if g.deleted > 0 {
                            let hole = g.v.buf.ptr().add(g.processed - g.deleted);
                            ptr::copy_nonoverlapping(current, hole, 1);
                        }
                        g.processed += 1;
                    }
                }
            }
        }

        pub fn dedup(&mut self)
        where
            T: PartialEq,
        {
            let len = self.len;
            if len <= 1 {
                return;
            }

            // Leak amplification: if `==` or a destructor panics we leak
            // instead of risking a double drop.
            self.len = 0;
            let p = self.buf.ptr();
            let mut write = 1;
            unsafe {
                for read in 1..len {
                    if &*p.add(read) == &*p.add(write - 1) {
                        ptr::drop_in_place(p.add(read));
                    } else {
                        if read != write {
                            ptr::copy_nonoverlapping(p.add(read), p.add(write), 1);
                        }
                        write += 1;
                    }
                }
            }
            self.len = write;
        }

        pub fn drain<R: RangeBounds<usize>>(&mut self, range: R) -> Drain<'_, T> {
            let len = self.len;
            let start = match range.start_bound() {
                Bound::Included(&n) => n,
                Bound::Excluded(&n) => n.checked_add(1).unwrap_or_else(|| capacity_overflow()),
                Bound::Unbounded => 0,
            };
            let end = match range.end_bound() {
                Bound::Included(&n) => n.checked_add(1).unwrap_or_else(|| capacity_overflow()),
                Bound::Excluded(&n) => n,
                Bound::Unbounded => len,
            };
            assert!(start <= end, "slice index starts at {start} but ends at {end}");
            assert!(end <= len, "range end index {end} out of range for slice of length {len}");

            unsafe {
                // Leak amplification: if Drain is mem::forget'd,
                // the tail is leaked, never double-dropped.
                self.len = start;
                let slice = slice::from_raw_parts(self.buf.ptr().add(start), end - start);
                Drain { tail_start: end, tail_len: len - end, iter: slice.iter(), vec: NonNull::from(&mut *self) }
            }
        }

        pub fn as_slice(&self) -> &[T] { unsafe { slice::from_raw_parts(self.buf.ptr(), self.len) } }
        pub fn as_mut_slice(&mut self) -> &mut [T] { unsafe { slice::from_raw_parts_mut(self.buf.ptr(), self.len) } }

        /// `new_len <= capacity` and the first `new_len` elements
        /// must be initialized.
        pub unsafe fn set_len(&mut self, new_len: usize) { self.len = new_len; }
    }

    impl<T> Deref for Vec<T> {
        type Target = [T];
        fn deref(&self) -> &Self::Target { self.as_slice() }
    }
}
