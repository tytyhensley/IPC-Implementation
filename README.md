# IPC Implementation.
## Barber Shop Synchronization
A barber shop has one barber, one barber chair, and n chairs for waiting customers, if any, to
sit on.
- If there are no customers present, the barber sits down in the barber chair and falls asleep
- When a customer arrives, he has to wake up the sleeping barber.
- If additional customers arrive while the barber is cutting a customer's hair, they either
sit down (if there are empty chairs) or leave the shop (if all chairs are full).

The problem is to program the barber and the customers without getting into race conditions.

**Write a program in C semaphores to implement this problem.**
