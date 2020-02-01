mod deck;
mod card;
mod rand;
mod cardpile;
mod player;
mod table;
mod dealer;
mod strategies;

use cardpile::CardPile;
use player::Player;
use table::Table;
use std::env;
use std::time::Instant;

fn main() {
    let args: Vec<String> = env::args().collect();

    const NUM_PLAYERS: i32 = 5;
    const NUM_DECKS: i32 = 8;
    const BET_SIZE:i32 = 10;
    const MIN_CARDS:usize = 40;

    let mut rounds:i32 = 100000;
    const VERBOSE:bool = false;

    if args.len() == 2 {
        rounds = args[1].parse::<i32>().unwrap();
    }

    let mut table1 = Table {..Table::new(NUM_PLAYERS,NUM_DECKS,BET_SIZE,MIN_CARDS,VERBOSE)};
    table1.m_cardpile.shuffle();

    let start = Instant::now();

    for x in 1..rounds+1 {
        if VERBOSE {
            println!("Round {}", x);
        }
        if !VERBOSE && rounds > 1000 && x % (rounds/100) == 0 {
            eprint!("\rProgress: {:.0}%", (x as f32/rounds as f32)*100.0);
        }

        table1.start_round();
        table1.check_earnings();
    }

    table1.clear();
    print!("\r");
    for player in table1.m_players.iter() {
        println!("Player {} earnings: {}\t\tWin Percentage: {}%", player.m_playernum, player.m_earnings as i32, 50.0 + (player.m_earnings/(rounds as f32 * BET_SIZE as f32) * 50.0));
    }
    println!("Casino earnings: {}", table1.m_casinoearnings);
    println!("Played {} rounds in {} seconds", rounds, start.elapsed().as_millis() as f32/1000.0);
}