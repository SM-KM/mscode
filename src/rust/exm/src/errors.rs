#[cfg(panic = "unwind")]
fn ah() {
    println!("Spit it out!!!!");
}

#[cfg(not(panic = "unwind"))]
fn ah() {
    println!("Spit it out!!!!");
}

fn drink(beverage: Option<&str>) {
    let inside = beverage.unwrap();
    if inside == "lemonade" {
        panic!("aAAaaAaaaAAAA")
    };
    println!("Love it, {}!", inside);
}

// option and unwrap
fn give_adlt(drink: Option<&str>) {
    match drink {
        Some("limonade") => println!("too sugary"),
        Some(inner) => println!("{}? nice", inner),
        None => println!("Nothing?, fuk u"),
    }
}

fn n_bit(curr_a: Option<u8>) -> Option<String> {
    let cur: u8 = curr_a? + 1;
    Some(format!("Next: {}", cur))
}

struct Person {
    job: Option<Job>,
}

#[derive(Clone, Copy)]
struct Job {
    phone_number: Option<PhoneNumber>,
}

#[derive(Clone, Copy)]
struct PhoneNumber {
    area_code: Option<u8>,
    number: u32,
}

impl Person {
    fn work_area_code(&self) -> Option<u8> {
        self.job?.phone_number?.area_code
    }
}

pub fn errors() {
    let water = Some("water");
    let void = None;

    give_adlt(water);
    give_adlt(void);

    drink(water);
    drink(void);

    n_bit(None);

    let p = Person {
        job: Some(Job {
            phone_number: Some(PhoneNumber { area_code: Some(61), number: 439222222 }),
        }),
    };

    assert_eq!(p.work_area_code(), Some(61));
}
