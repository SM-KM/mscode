#![allow(dead_code)]
trait Animal {}
struct Bee {}
impl Animal for Bee {}

fn rnd_anim(_dn: f64) -> Box<dyn Animal> {
    Box::new(Bee {})
}

// implementing the traits inside core::ops, allows to
// perform operator overloading

// iterators
struct Fibonacci {
    curr: u32,
    next: u32,
}

impl Iterator for Fibonacci {
    type Item = u32;
    fn next(&mut self) -> Option<Self::Item> {
        let current = self.curr;
        self.curr = self.next;
        self.next = current + self.next;
        Some(current)
    }
}

pub fn traits() {}
