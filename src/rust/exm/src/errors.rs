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

pub fn errors() {
    let water = Some("water");
    let void = None;

    give_adlt(water);
    give_adlt(void);

    drink(water);
    drink(void);
}
