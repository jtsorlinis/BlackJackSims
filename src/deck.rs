use crate::card::Card;
// use crate::rand;

static RANKS: [&str; 13] = [
    "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K",
];
static SUITS: [&str; 4] = ["Clubs", "Hearts", "Spades", "Diamonds"];

pub struct Deck {
    pub m_cards: Vec<*mut Card>,
}

impl Deck {
    pub fn new() -> Deck {
        Deck {
            m_cards: Deck::generate_deck(),
        }
    }

    fn generate_deck() -> Vec<*mut Card> {
        let mut vec: Vec<*mut Card> = Vec::new();
        for suit in SUITS.iter() {
            for rank in RANKS.iter() {
                let card = Box::into_raw(Box::new(Card::new(rank, suit)));
                vec.push(card);
            }
        }
        return vec;
    }

    // pub fn print(&self) -> String {
    //     self.m_cards[0].print();
    //     let mut output = String::default();
    //     for card in self.m_cards.iter() {
    //         output += card.print();
    //         output += "\n";
    //     }

    //     return output;
    // }

    // pub fn shuffle(&mut self) {
    //     let mut rng = rand::Rand::new(0);
    //     rng.shuffle(&mut self.m_cards);
    // }
}
