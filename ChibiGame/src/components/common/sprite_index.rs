#[derive(Debug, Default, PartialEq, Eq, Clone)]
pub struct SpriteIndex {
    pub column: u32,
    pub row: u32,
    pub inverted: bool,
}
