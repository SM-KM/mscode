#![allow(unused, dead_code)]

pub mod vec {
    use core::marker::PhantomData;
    use core::ptr::NonNull;

    use core::cmp;
    use std::{
        alloc::{Layout, alloc, dealloc, handle_alloc_error, realloc},
        mem, ptr,
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
    }
}
