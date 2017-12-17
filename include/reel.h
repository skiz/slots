#ifndef REEL_H
#define REEL_H

/*
 * The visually moving part of the machine.
 * It contains references to the images and animations used.
 *
 * wtf does a reel do? 
 *  * has many pictures
 *  * is told what to display
 *  * displays the results
 *  * doesnt give a shit.
 *  * It likes to spin, drop or whateever...
 *  * no events
 *  * StopAt(??)
 *
 *  not controlled, needs to be...
 *
 *  what about misalignments, etc.
 *
 *  Another subsystem needs to create the drop
 *  so that the results are already verified.
 *
 *  given 2,5,5,1,2 it should stop as told visually.  
 *
 *  rng handles making decisions
 *
 *  "looseness" should be configurable.
 *
 *
 *  
 *
 *
 *
 *
 */
class Reel {
  public:
    void Init();
};

#endif
