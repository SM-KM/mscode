#![allow(unused, dead_code)]

pub mod vec {
    use core::marker::PhantomData;
    use core::ptr::NonNull;

    use core::cmp;
    use std::{
        alloc::{Layout, alloc, dealloc, handle_alloc_error, realloc},
        mem,
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

        pub fn capacity(&self) -> usize;
    }
}
