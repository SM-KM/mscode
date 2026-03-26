#![allow(dead_code)]
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

// conbinators = map
enum Food {
    Apple,
    Carrot,
    Tomato,
}

struct Peeled(Food);
struct Cooked(Food);
struct Chopped(Food);

fn process(food: Option<Food>) -> Option<Cooked> {
    food.map(|f| Peeled(f)).map(|Peeled(f)| Chopped(f)).map(|Chopped(f)| Cooked(f))
}

#[derive(Debug)]
enum Fruit {
    Apple,
    Orange,
    Banana,
    Kiwi,
    Lemon,
}

pub fn errors() {
    let p = Person {
        job: Some(Job {
            phone_number: Some(PhoneNumber { area_code: Some(61), number: 439222222 }),
        }),
    };

    assert_eq!(p.work_area_code(), Some(61));
    let apple = Some(Fruit::Apple);
    let orange = Some(Fruit::Orange);
    let no_fruit: Option<Fruit> = None;

    let first_available_fruit = no_fruit.or(orange).or(apple);
    println!("first_available_fruit: {:?}", first_available_fruit);

    let mut my_fruit: Option<Fruit> = None;
    let apple = Fruit::Apple;

    let f_ava = my_fruit.get_or_insert(apple);
    println!("first_available_fruit is: {:?}", f_ava);
}
