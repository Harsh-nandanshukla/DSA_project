#  Dynamic Task Scheduling using Interval Trees

This C++ project implements a **dynamic task scheduling system** using an **augmented Red-Black Interval Tree**. The goal is to manage concurrent time-bound tasks across multiple workers (or processors), ensuring high-priority tasks are always scheduled, and lower-priority tasks can be preempted if needed.


##  Features

-  Insert intervals with conflict detection
-  Automatically assign tasks to available workers
-  Preempt low-priority tasks on conflict
-  Efficient interval deletion
-  In-order display of all intervals in the tree
-  Overlap detection between any two intervals
-  Fully balanced using Red-Black Tree logic (O(log n) insert/delete)

---

##  Use Cases

This system is ideal for:
- Operating system CPU/task schedulers
- Reservation systems (e.g. room or facility booking)
- Network bandwidth allocation
- Calendar or event conflict resolution
- Multi-threaded job managers


##  How It Works

- Intervals are stored in a **Red-Black Tree** augmented with a `MaxRight` field to allow efficient overlap checks.
- Each interval is tied to a `worker ID`, simulating parallel resource usage.
- On insertion:
  - If a free worker is available, the interval is inserted normally.
  - If all workers are busy:
    - It tries to preempt a currently running task with **lower priority**.
    - If no such task is found, the insertion is rejected.
- Intervals are always managed efficiently due to logarithmic tree operations.


Make sure you have a C++ compiler (g++) installed.

```bash
g++ -o scheduler Interval_tree.cpp
