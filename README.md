Slot Machine
------------
Multi reel slot machine designed for Raspberry Pi

Written in SDL C++


Hopeful Features
----------------
* Bill/Ticket Acceptor Support
* Ticket Printer Support
* Bonus Games
* Progressive Jackpot Support
* Custom Wheel Art & Animations
* Custom Sound Effects & Music
* Hardware Random Number Generator Support
* Accounting System
* Diagnostic Mode
* Configurable Payout Ratio
* Configurable Hit Frequency
* Hardware output for Credit Counter
* Hardware input for credit buttons
* Power Failure Protection - Keep credits on hard power off.
* Tamper Detection (Tilt)- Software monitoring and hardware inputs.
* Weighted Symbols
* Random Bonus Games Triggers
* Bonus Game Touchscreen Support
* Help Pages
* Attendant Button Hardware Support
* Drop or rotate(spin) wheels
* Close Call Spins (slow reels)

RNG are selected when spin is hit (1 per reel)


Notes
-----

Machine has many Wheels

Wheel are for display purposes only and provide
animation routines.

The symbol allocations are based on the RNG and config settings.


real slot machine flow:
spin button is pushed
5 random numbers are selected (1 per wheel)
A final combination for a payout is pre-selected as the result.
Each wheel shows random symbols until it lands on the preselected one.
winnings are preselected and credited.


better flow:
spin button is pushed
5 random numbers are selected (1 per wheel) and weighted symbols are selected
Each wheel is loaded with a circular list of random symbols existing + (circular list)
The wheels spin, and symbols are landed on & the list is pruned.
The final payout is calculated based on pay lines and the multiplier.
Bonus game, etc is checked and started as needed (new state).



:/ each number defines what symbols will appear on each wheel(?)

random number generated between 0 and N (billions?)
each symbol is given a value worst to best on a logarithmic scale

approach 1)
uniformly random in log R given N buckets, each symbol representing
N buckets down to 1 bucket (JACKPOT)


approach 2)
using the RNG, decide if they won, and how much. Fill the spools with 
the symbols requied for that win.



Q) how do we do "near miss" and "slow hit"?

Strategy Pattern - Select a type of result for the spin
Command Pattern - 


Q) Should we support "skill stop"?  - its a hoax mostly but we can.

ie:

slow miss: 2 wheels hit diagnal on bonus, bonus spins by and misses by 1
slow hit: 2 wheels hit on a payline, third spins slow and hits


near jackpot miss:  lots of stuff lines up for a jackpot, but ensures a miss.


full screen scare: lots of winning symbol sets spin by in groups, but don't hit enough.


WheelSupervisor
------------------------
We got N wheels (maybe wheel is a bad name?).
these are visual representations, and are designed
to be instructed to select a strategy to show images.
(slow, fast, lots of win, simple symbol spins, etc).
Each wheel is instructed to land in a specific pattern.

N wheel (visual) are handled by a wheel controller.
Wheel controller tells each wheel where to stop, and what
symbols to show while it spins, and what symbols it should
show when stopped.


Wheel is dumb. No logic in the wheel but animations.

Controller directs N wheels.

shares pointers to symbols/animations, tells wheels what strategy to use, and
gives access to the audio system.

Wheel Strategies
----------------
* Normal Spin
* Power Spin
* Slow Miss
* Slow Hit


slow - on a win)
wheel 1) fast hit
wheel 2) (music changes), slow hit (slow/near hit rest of wheels)
wheel ..N) hits (flashes)

slow - on a near miss)

Q) do the wheels make noise or does the manager do it for them? (thinking manager)


Example - Hit
-------------
Player hits spin.
The RNG says pay 4x bet.
4x is calculated to be 3 bars.
The position is calculated to be diagonal across 3.
Random symbols are shown spinning.
First hit) a bar is shown in pos {1,1}
Second hit) a bar is shown in pos {2,2}
Third hit) a bar is shown in pos {3,3}
No additional payouts are shown.
Player gets 4x bet on bars.

Example - Near Miss
-------------------
Player hits spin.
The RNG sats "near miss jackpot".
The positions of a near miss are calculated.



5 wheels is the standard for video slots
----------------------------------------
Why? fukifIknow...




/*
    TODO: make a payout table for each winning combination
     ie:

	3A = 50 * Bet, 4A = 100 * Bet, 5A = 500 * Bet
	3K = 20 * Bet, 4K = 50 * Bet, 5K = 100 * Bet
	

  BUILDING THE REELS:

    Each stage uses a weighted list to determine the wheel(s) and their
    target position.


    Stage 1:  Predefined Outcome with target positions (all reels defined)
      Jackpot       1
      Bonus        50
      Near Miss    70
      Standard  10000

    Stage 2:  Predefined Reel without target positions (1 for each reel)
      Jackpot       5
      Bonus       400
      All A       500
      All K       600
      All Q       700
      Standard  10000

    Stage 3:  Standard Wheel Generation (each face is weighted and selected)
       JACKPOT:   50
         BONUS: 1000
	  WILD: 1200
	     A: 3000
             K: 3100
	     Q: 3200
	     J: 3450
	    10: 3450
	CHERRY: 3450
	   BAR: 3450

	calculate sum of all weights
	pick random number between 0 and sum
	loop through lookup
	    if sum > weight
	      sum -= weight
	    else
	      select this one


    Stage 4:  Define stop positions for all reels that don't have them

*/

