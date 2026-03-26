#![allow(dead_code)]
trait Animal {}
struct Bee {}
impl Animal for Bee {}

fn rnd_anim(_dn: f64) -> Box<dyn Animal> {
    Box::new(Bee {})
}

pub fn traits() {}
