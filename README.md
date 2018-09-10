# Caltech CS2 Assignment 9: Othello Bot sudormrf

By Nicholas Ardavin and Alessio Amaolo

## Bot Features

- Bitboard implemtation of Othello board for fast evaluation of board state and efficient storage
- Principal Variation Search (Negascout) with Negamax to reduce search time of minimax
- Basic move ordering with 1-ply search
- 10-ply search for accurate lookahead
- Time-dependent heuristic function that accounts for piece count, number of available moves, number of stable pieces, frontier size, and board parity
- Machine learninng optimization of heuristic parameters using TD-Leaf(λ)
- 20-ply endgame solver

## Future Plans

- Opening book
- Replace current tree search with probcut
- Improve move ordering
- Iterative deepening
- Optimize time usage

## Submission Log

Winter 2018 Submission: [979b3447eef28d7ca8fba07480eb7cd0e82f997f](https://github.com/nardavin/OthelloBot/tree/979b3447eef28d7ca8fba07480eb7cd0e82f997f)

## Relevant Papers/Websites

[Othello Strategy Guide](http://radagast.se/othello/Help/strategy.html)

[Bitboard Methods for Games](http://eprints.qut.edu.au/85005/1/__staffhome.qut.edu.au_staffgroupm$_meaton_Desktop_bits-7.pdf)

[Negamax - Wikipedia](https://en.wikipedia.org/wiki/Negamax)

[MTD-f Search Function - Wikipedia](https://en.wikipedia.org/wiki/MTD-f)

[Probcut: An Effective Extension of the alpha-beta Algorithm](http://wiki.cs.pdx.edu/cs542/papers/buro/probcut.pdf)

[TDLeaf(λ): Combining Temporal Difference Learning with Game-Tree
Search](https://arxiv.org/pdf/cs/9901001.pdf)

See [assignment9_part1.html](http://htmlpreview.github.io/?https://github.com/caltechcs2/othello/blob/master/assignment9_part1.html) and [assignment9_part2.html](http://htmlpreview.github.io/?https://github.com/caltechcs2/othello/blob/master/assignment9_part2.html) for assignment details.
