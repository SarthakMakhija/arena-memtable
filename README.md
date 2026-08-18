# arena-memtable [![arena-memtable-ci](https://github.com/SarthakMakhija/arena-memtable/actions/workflows/build.yml/badge.svg)](https://github.com/SarthakMakhija/arena-memtable/actions/workflows/build.yml)
A tiny arena-backed sorted memtable in C, exploring arena allocation, raw byte storage, concurrency, and storage-engine data structures.

## Goals of the Project
- **Arena-based Memtable**: Build a custom memory-arena-backed Memtable on top of a sorted linked list for educational purposes.
- **API Operations**: Support standard `put` (insert/update) and `get` (retrieve) operations.
- **Thread Safety**: Make the memory arena and list operations thread-safe.
- **Exhaustive Testing**: Maintain high reliability and verify correctness through exhaustive test coverage.

## Components

### 1) Memtable
The `memtable` (Memory Table) is the main write buffer wrapper. In LSM-tree storage engines, all incoming writes (`put`) are buffered in a memtable first before being flushed to disk. It exposes a clean `put`/`get` API and delegates the underlying storage and ordering to the sorted list.

### 2) Arena
The `arena` is a contiguous block of pre-allocated memory (`unsigned char*`). Rather than allocating nodes dynamically on the system heap using expensive individual `malloc` calls, the arena bump-allocates space by advancing an offset. Instead of returning raw pointers, the allocator returns relative offsets (`arena_offset`), facilitating easy serialization and relocation.

### 3) Node
A `node` is a purely logical concept in the codebase. Each node wraps a reference to the arena and the starting offset where its key/value pair is serialized.
```
Binary Layout of a Node at 'nodeOffset' in Arena:

 0             2             6                   10               10 + KeyLen        Total Size
+-------------+-------------+-------------------+-----------------+------------------+
| Key Length  | Val Length  | Next Node Offset  | Key Payload     | Value Payload    |
| (uint16_t)  | (uint32_t)  | (arena_offset)    | (KeyLen bytes)  | (ValLen bytes)   |
| [ 2 Bytes ] | [ 4 Bytes ] | [ 4 Bytes ]       |                 |                  |
+-------------+-------------+-------------------+-----------------+------------------+
|<------------------- Fixed Header (10 Bytes) ------------------->|
```

### 4) Interaction
- **Memtable** receives public lookup/insert requests and delegates them to the **Sorted List**.
- **Sorted List** maintains the logic of a single-linked list (traversal, finding insertion points, and updating next links).
- **Node** is a logical concept that wraps the arena and starting offset. It acts as the serialization layer to format key-value layouts directly inside the **Arena**.
- **Arena** handles space allocations within its contiguous backing buffer, returning relative offsets.


## Pending Items
- [ ] **Thread-safe Arena**: Make next offset bump allocation atomic using atomic operations.
- [ ] **Lock-free / Thread-safe Linked List Insertion**: Atomically update the next pointer (`next_offset`) while inserting key/value pairs in the sorted list.