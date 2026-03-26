#![allow(dead_code)]
trait Animal {}
struct Bee {}
impl Animal for Bee {}

fn rnd_anim(_dn: f64) -> Box<dyn Animal> {
    Box::new(Bee {})
}

// implementing the traits inside core::ops, allows to
// perform operator overloading

pub fn traits() {}
