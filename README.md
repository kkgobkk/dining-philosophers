# Dining Philosophers
A simple simulation where N "philosopher" processes and N "chopstick" semaphores are created.
Each philosopher infinitely alternates between "eating" and "thinking" for a short time.
Philosopher i needs chopsticks i and (i+1)%N to start eating. When a philosopher stops eating, it puts down both chopsticks.
The simulation ends automatically after a certain time.

For more information, check https://en.wikipedia.org/wiki/Dining_philosophers_problem

## Types of philosophers
Right now, there is only 1 type of philosopher process, but my goal is to implement several different solutions that avoid
problems such as deadlock and starvation.

* basic: tries to pick up the left chopstick. After it acquires the left chopstick, it tried to pick up the right chopstick. Never puts down
  a chopstick unless it just finished eating. Every philopher starts in "eating" mode. This solution is prone to deadlock and starvation
  (imagine what would happen if every philosopher immediately picked up the chopstick to their left)

* coming soon

## Compiling and executing
To compile simply run "make all" in the main directory.

To run, you can use `make run`, or `./bin/dining_philosophers N` if you want to specify N manually.
Both of these commands must be ran from the main directory
