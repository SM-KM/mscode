#![allow(unused, dead_code)]

pub mod vec {
    use core::marker::PhantomData;
    use core::ptr::NonNull;

    /// the buffer pointer + capacity, knows how to grow, and owns the allocations
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
}
