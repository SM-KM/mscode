#![allow(unused, dead_code)]

pub mod vec {
    use core::marker::PhantomData;
    use core::ptr::NonNull;

    /// the buffer pointer + capacity, knows how to grow,
    /// and owns the allocations
    pub struct RawVec<T> {
        ptr: NonNull<T>,
        cap: usize,
    }
    pub struct Vec<T> {
        buf: RawVec<T>,
        len: usize,
    }

    pub struct IntoIter<T> {
        buf: NonNull<T>,
        cap: usize,
        ptr: *const T,
        end: *const T,
    }

    impl<T> RawVec<T> {
        pub const fn new() -> Self;
        pub fn with_capacity(cap: usize) -> Self;
        pub fn ptr(&self) -> *mut T;
        pub fn capacity(&self) -> usize;
        fn grow_amortized(&mut self, len: usize, additional: usize);
        pub fn grow_one(&mut self);
        pub fn reserve(&mut self, len: usize, additional: usize);
        pub fn shrink_to_fit(&mut self, cap: usize);
    }

    impl<T> Drop for RawVec<T> {
        fn drop(&mut self) {}
    }

    impl<T> vec<T> {
        pub const fn new() -> Self;
    }
}
