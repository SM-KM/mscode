#![allow(unused, dead_code)]

pub mod vec {
    use core::marker::PhantomData;
    use core::ptr::NonNull;

    use core::cmp;
    use std::{
        alloc::{Layout, alloc, dealloc, handle_alloc_error},
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

        pub fn ptr(&self) -> *mut T;
        pub fn capacity(&self) -> usize;
        fn grow_amortized(&mut self, len: usize, additional: usize);
        pub fn grow_one(&mut self);
        pub fn reserve(&mut self, len: usize, additional: usize);
        pub fn shrink_to_fit(&mut self, cap: usize);
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
        pub fn with_capacity(capacity: usize) -> Self;
        pub unsafe fn from_raw_parts(ptr: *mut T, length: usize, capacity: usize) -> Self;
        pub fn capacity(&self) -> usize;
    }
}
