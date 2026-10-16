pub mod vector {
    use core::marker::PhantomData;
    use core::ptr::NonNull;

    /// the buffer pointer + capacity, knows how to grow, and owns the allocations
    pub struct RawVec<T> {
        ptr: NonNull<T>,
        cap: usize,
    }
}
