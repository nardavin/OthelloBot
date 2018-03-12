# Caltech CS2 Assignment 9: Othello Bot sudormrf

By Nicholas Ardavin and Alessio Amaolo

## Contributions by Each Member

Nicholas Ardavin:
-Implemented minimax and AB pruning
-Improved minimax to use negamax and negascout
-Implemented endgame solver
-Completed the bitboard implementation
-Debugged lots of code

Alessio Amaolo
-Wrote basic testing code and basic structure for program
-Began to implement the bitboard usage instead of the given board
-Wrote and tuned heuristic
-Tested heuristic/minimax against many puzzles in the book "Othello: A Minute to Learn, A Lifetime to Master" to ensure that it was finding the right solutions
-Read papers to find the best algorithms/implementations for bitboard, negascout, probcut. 
-Attempted the integration of Logistello's opening book. 
-Debugged a fair amount of code


## Improvements to our bot

Negascout with Negamax is better than minimax with alpha beta pruning, since it does move ordering to search the moves that are likely better significantly more than the moves that are likely to not be as good. 

Endgame solver that looks 20 moves ahead and forces the opponent to lose (if a solution is found). Otherwise it'll play normally with the heuristic and continue searching for a forced win, since the opponent may still make a mistake. 

Bitboards instead of the regular board implementation. This utilises techniques from a paper online () to generate moves, make moves, and calculate all the heuristic parameters (mainly stability). Since we could use bit operations to do all these tasks, we got a significant speed increase (factor of about 100 or 2 extra depth). 

Our heuristic took into account mobility, frontiers, stable pieces, parity. Then we tested it against puzzles in the book "Othello: A Minute to Learn, A Lifetime to Master" to ensure it was giving us accurate values. 

We tried to use an opening book but it is hard. Maybe next year? 


## Future Work

Opening book would significantly speed up the beginning of the game, but we didn't have time to implement this. 

Probcut would likely do better pruning. 

We should have used iterative deepening to search further and to also use all our time. In the first tournament we used 8 minutes total and in the second we used approximately 1 minute per game. 

## Papers read

NOTE: we did not look at source code.

https://web.stanford.edu/class/cs221/2017/restricted/p-final/man4/final.pdf
http://eprints.qut.edu.au/85005/1/__staffhome.qut.edu.au_staffgroupm$_meaton_Desktop_bits-7.pdf
https://skatgame.net/mburo/ps/improve.pdf
https://en.wikipedia.org/wiki/Negamax
https://en.wikipedia.org/wiki/MTD-f
http://wiki.cs.pdx.edu/cs542/papers/buro/probcut.pdf



See [assignment9_part1.html](http://htmlpreview.github.io/?https://github.com/caltechcs2/othello/blob/master/assignment9_part1.html) and [assignment9_part2.html](http://htmlpreview.github.io/?https://github.com/caltechcs2/othello/blob/master/assignment9_part2.html)
