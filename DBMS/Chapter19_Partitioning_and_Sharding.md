# Chapter 19: Partitioning & Sharding in DBMS 🗂️

## Introduction 📋

**A big problem can be solved easily when it is chopped into several smaller sub-problems. That is what the partitioning technique does. It divides a big database containing data metrics and indexes into smaller and handy slices of data called partitions.**

### Core Concept
**The partitioned tables are directly used by SQL queries without any alteration. Once the database is partitioned, the data definition language can easily work on the smaller partitioned slices, instead of handling the giant database altogether.**

### Key Benefit
**This is how partitioning cuts down the problems in managing large database tables.**

---

## Partitioning Fundamentals 🔍

### What is Partitioning?
**Partitioning is the technique used to divide stored database objects into separate servers. Due to this, there is an increase in performance, controllability of the data. We can manage huge chunks of data optimally.**

### Horizontal Scaling Integration
**When we horizontally scale our machines/servers, we know that it gives us a challenging time dealing with relational databases as it's quite tough to maintain the relations. But if we apply partitioning to the database that is already scaled out i.e. equipped with multiple servers, we can partition our database among those servers and handle the big data easily.**

### Partitioning Visualization
```
Large Database Table:
┌─────────────────────────────────────────────────────────────┐
│                    GIANT TABLE (1M rows)                   │
│  [1] [2] [3] [4] [5] ... [999,996] [999,997] [999,998] [999,999] [1,000,000]  │
└─────────────────────────────────────────────────────────────┘
                                ↓
                        Partitioning Applied
                                ↓
┌─────────────┐ ┌─────────────┐ ┌─────────────┐ ┌─────────────┐
│ Partition 1 │ │ Partition 2 │ │ Partition 3 │ │ Partition 4 │
│ (250K rows) │ │ (250K rows) │ │ (250K rows) │ │ (250K rows) │
└─────────────┘ └─────────────┘ └─────────────┘ └─────────────┘
     ↓ Server 1       ↓ Server 2       ↓ Server 3       ↓ Server 4
```

---

## Types of Partitioning 📊

### 1. Vertical Partitioning 📊
**Slicing relation vertically / column-wise.**

**How It Works**:
```
Original Table:
┌─────────┬─────────┬─────────┬─────────┬─────────┐
│   ID    │  Name   │  Email  │ Address │  Phone  │
├─────────┼─────────┼─────────┼─────────┼─────────┤
│   1     │  John   │john@e.com│ 123 St  │555-1234 │
│   2     │  Jane   │jane@e.com│ 456 Ave │555-5678 │
└─────────┴─────────┴─────────┴─────────┴─────────┘

After Vertical Partitioning:
┌─────────┬─────────┬─────────┐    ┌─────────┬─────────┐
│   ID    │  Name   │  Email  │    │   ID    │ Address │  Phone  │
├─────────┼─────────┼─────────┤    ├─────────┼─────────┼─────────┤
│   1     │  John   │john@e.com│    │   1     │ 123 St  │555-1234 │
│   2     │  Jane   │jane@e.com│    │   2     │ 456 Ave │555-5678 │
└─────────┴─────────┴─────────┘    └─────────┴─────────┴─────────┘
        ↓ Server 1                      ↓ Server 2
```

**Key Characteristics**:
- **Columns divided** across different servers
- **Need to access different servers** to get complete tuples
- **Useful for wide tables** with many columns
- **Reduces I/O** for queries that need only specific columns

**Use Cases**:
- **User profile data** (basic info vs detailed info)
- **Product catalog** (basic info vs specifications)
- **Audit logs** (current data vs historical data)

### 2. Horizontal Partitioning 📊
**Slicing relation horizontally / row-wise.**

**How It Works**:
```
Original Table:
┌─────────┬─────────────┬─────────────┐
│   ID    │    Name     │    City     │
├─────────┼─────────────┼─────────────┤
│   1     │    John     │    NYC      │
│   2     │    Jane     │    LA       │
│   3     │    Mike     │    NYC      │
│   4     │    Sara     │    Chicago  │
└─────────┴─────────────┴─────────────┘

After Horizontal Partitioning:
┌─────────┬─────────────┬─────────────┐    ┌─────────┬─────────────┬─────────────┐
│   ID    │    Name     │    City     │    │   ID    │    Name     │    City     │
├─────────┼─────────────┼─────────────┤    ├─────────┼─────────────┼─────────────┤
│   1     │    John     │    NYC      │    │   4     │    Sara     │    Chicago  │
│   2     │    Jane     │    LA       │    │   5     │    Tom      │    Miami    │
└─────────┴─────────────┴─────────────┘    └─────────┴─────────────┴─────────────┘
        ↓ Server 1                              ↓ Server 2
```

**Key Characteristics**:
- **Rows divided** across different servers
- **Independent chunks of data tuples** stored in different servers
- **Each partition contains complete rows**
- **Easier to manage** than vertical partitioning

**Common Partitioning Strategies**:
- **Range-based**: Partition by value ranges (e.g., dates, IDs)
- **Hash-based**: Partition using hash function
- **List-based**: Partition by predefined value lists
- **Composite**: Combination of multiple strategies

---

## When to Apply Partitioning? 🎯

### Key Indicators

#### 1. **Dataset Size Issues**
**Dataset becomes much huge that managing and dealing with it becomes a tedious task.**

**Symptoms**:
- **Table size** > 10GB
- **Row count** > 10 million
- **Index size** becoming problematic
- **Backup/restore** operations taking too long

#### 2. **Performance Bottlenecks**
**The number of requests are enough larger that the single DB server access is taking huge time and hence the system's response time becomes high.**

**Symptoms**:
- **Query response time** increasing significantly
- **Database CPU usage** consistently high
- **I/O bottlenecks** on disk operations
- **Memory pressure** due to large datasets

**Decision Matrix**:
| **Database Metric** | **Partitioning Recommended** | **Action Required** |
|--------------------|------------------------------|--------------------|
| **Table Size** > 10GB | ✅ Yes | Consider partitioning |
| **Row Count** > 10M | ✅ Yes | Plan partitioning strategy |
| **Query Time** > 5s | ✅ Yes | Implement partitioning |
| **CPU Usage** > 80% | ⚠️ Consider | Evaluate partitioning |
| **Memory Usage** > 90% | ⚠️ Consider | Check partitioning benefits |

---

## Advantages of Partitioning ✅

### 1. Parallelism ⚡
**Multiple partitions can be queried simultaneously**

```
Single Table Query:
┌─────────────────────────────────────────┐
│           Large Table Query              │
│  SELECT * FROM large_table WHERE ...    │
│           ↳ 30 seconds                  │
└─────────────────────────────────────────┘

Partitioned Query:
┌─────────────┐ ┌─────────────┐ ┌─────────────┐
│ Partition 1 │ │ Partition 2 │ │ Partition 3 │
│  8 seconds  │ │  8 seconds  │ │  8 seconds  │
└─────────────┘ └─────────────┘ └─────────────┘
               ↳ Parallel Execution → 8 seconds total
```

### 2. Availability 🛡️
**If one partition fails, others remain accessible**

```
Partition Failure Scenario:
┌─────────────┐ ┌─────────────┐ ┌─────────────┐
│ Partition 1 │ │ Partition 2 │ │ Partition 3 │
│   Active    │ │   Failed    │ │   Active    │
└─────────────┘ └─────────────┘ └─────────────┘
       ↓              ↓              ↓
   Available      Unavailable    Available
       ↳ Partial System Availability ✅
```

### 3. Performance 🚀
**Smaller partitions = faster queries**

```
Performance Comparison:
Single Table:    100M rows → 2 minutes query time
Partitioned:     4 × 25M rows → 30 seconds query time
Performance Gain: 75% faster ✅
```

### 4. Manageability 🛠️
**Easier maintenance operations**

- **Backup/Restore**: Work with smaller datasets
- **Index Rebuilding**: Faster on smaller partitions
- **Data Archiving**: Archive entire partitions
- **Schema Changes**: Apply to specific partitions

### 5. Cost Reduction 💰
**Reduce Cost, as scaling-up or vertical scaling might be costly.**

**Cost Comparison**:
```
Vertical Scaling:
┌─────────────────────────────────────────┐
│        High-End Server                  │
│      32 Cores, 256GB RAM, 10TB SSD     │
│              Cost: $50,000              │
└─────────────────────────────────────────┘

Horizontal Scaling (Partitioned):
┌─────────┐ ┌─────────┐ ┌─────────┐ ┌─────────┐
│ Server 1│ │ Server 2│ │ Server 3│ │ Server 4│
│8C,64GB  │ │8C,64GB  │ │8C,64GB  │ │8C,64GB  │
│ $8,000  │ │ $8,000  │ │ $8,000  │ │ $8,000  │
└─────────┘ └─────────┘ └─────────┘ └─────────┘
Total Cost: $32,000 (36% savings) 💰
```

---

## Distributed Database 🌐

### What is Distributed Database?
**A single logical database that is, spread across multiple locations (servers) and logically interconnected by network.**

### Relationship with Optimization Techniques
**This is the product of applying DB optimisation techniques like Clustering, Partitioning and Sharding.**

```
Distributed Database Architecture:
┌─────────────────────────────────────────────────────────────┐
│                  Distributed Database                       │
│                                                           │
│  Location A         Location B         Location C         │
│  ┌─────────────┐   ┌─────────────┐   ┌─────────────┐      │
│  │   Node 1    │   │   Node 4    │   │   Node 7    │      │
│  │ (Partition) │   │ (Partition) │   │ (Partition) │      │
│  └─────────────┘   └─────────────┘   └─────────────┘      │
│  ┌─────────────┐   ┌─────────────┐   ┌─────────────┐      │
│  │   Node 2    │   │   Node 5    │   │   Node 8    │      │
│  │ (Replica)   │   │ (Replica)   │   │ (Replica)   │      │
│  └─────────────┘   └─────────────┘   └─────────────┘      │
│                                                           │
│  └───────────────────── Network ────────────────────────┘
└─────────────────────────────────────────────────────────────┘
```

### Why Distributed Database is Needed?
**Same reasons as partitioning (Refer to point 5 above):**
- **Large dataset management**
- **High request volume handling**
- **Performance optimization**
- **Scalability requirements**
- **Geographic distribution needs**

---

## Sharding: Advanced Horizontal Partitioning 🎯

### What is Sharding?
**Technique to implement Horizontal Partitioning.**

**The fundamental idea of Sharding is the idea that instead of having all the data sit on one DB instance, we split it up and introduce a Routing layer so that we can forward the request to the right instances that actually contain the data.**

### Sharding Architecture
```
Traditional Single Database:
Application → [Single DB Instance] → Response

Sharded Database:
           ┌─────────────────────────────────────┐
           │          Application                │
           └─────────────────────────────────────┘
                        ↓
           ┌─────────────────────────────────────┐
           │         Routing Layer               │
           │    (Determines shard location)      │
           └─────────────────────────────────────┘
                        ↓
        ┌───────────┬───────────┬───────────┬───────────┐
        │  Shard 1  │  Shard 2  │  Shard 3  │  Shard 4  │
        │(DB Instance)│(DB Instance)│(DB Instance)│(DB Instance)│
        └───────────┴───────────┴───────────┴───────────┘
```

### Sharding vs Partitioning
| **Aspect** | **Partitioning** | **Sharding** |
|------------|------------------|--------------|
| **Scope** | Within single database | Across multiple databases |
| **Implementation** | Database feature | Application-level design |
| **Transparency** | Transparent to queries | Requires routing logic |
| **Complexity** | Lower | Higher |
| **Scalability** | Limited | Unlimited |

### Advantages of Sharding ✅

#### 1. Scalability 📈
**Linear scaling with data growth**

```
Sharding Scalability:
1 Shard:    1M users  → 1000 QPS
2 Shards:   2M users  → 2000 QPS
4 Shards:   4M users  → 4000 QPS
8 Shards:   8M users  → 8000 QPS
Linear Growth ✅
```

#### 2. Availability 🛡️
**Multi-shard failure tolerance**

```
Shard Failure Impact:
┌─────────────┐ ┌─────────────┐ ┌─────────────┐
│  Shard A    │ │  Shard B    │ │  Shard C    │
│   Active    │ │   Failed    │ │   Active    │
└─────────────┘ └─────────────┘ └─────────────┘
       ↓              ↓              ↓
   Available      Unavailable    Available
       ↳ 66% System Availability ✅
```

### Disadvantages of Sharding ❌

#### 1. Complexity 🔧
**Implementation and maintenance challenges**

- **Partition mapping** complexity
- **Routing layer** to be implemented in the system
- **Non-uniformity** that creates necessity of Re-Sharding
- **Cross-shard queries** complexity

#### 2. Query Limitations 🚫
**Not well suited for Analytical type of queries**

**Scatter-Gather Problem**:
```
Analytics Query Example:
"SELECT COUNT(*) FROM users WHERE country IN ('USA', 'UK', 'India')"

Problem:
- Data scattered across different DB instances
- Must query all shards → Scatter
- Aggregate results from all shards → Gather
- High latency and complexity
```

---

## Case Study: Database Scaling Patterns 🚀

### Introduction to Cab Booking App Case Study

#### **Initial Stage: Tiny Startup**
```
Business Setup:
• Tiny startup
• ~10 customers onboard
• A single small machine DB stores all customers, trips, locations, booking data, and customer trip history
• ~1 trip booking in 5 mins
```

**Architecture**:
```
┌─────────────────────────────────────────┐
│           Single Small Machine          │
│  ┌─────────────────────────────────────┐ │
│  │        Single Database             │ │
│  │  ┌─────────┬─────────┬─────────────┐ │ │
│  │  │Customers│ Trips   │ Locations   │ │ │
│  │  └─────────┴─────────┴─────────────┘ │ │
│  └─────────────────────────────────────┘ │
└─────────────────────────────────────────┘
```

### Pattern 1: Query Optimisation & Connection Pool Implementation

#### **Problem Begins**
```
• Requests scales upto 30 bookings per minute
• Your tiny DB system has started performing poorly
• API latency has increased a lot
• Transactions facing Deadlock, Starvation, and frequent failure
• Sluggish App experience
• Customer dissatisfaction
```

#### **Solution Implementation**
```
Optimisation Techniques:
• Cache frequently used non-dynamic data like:
  - Booking history
  - Payment history
  - User profiles etc.
• Introduce Database Redundancy (Or may be use NoSQL)
• Use connection pool libraries to Cache DB connections
• Multiple application threads can use same DB connection
```

**Architecture**:
```
┌─────────────────────────────────────────┐
│              Application                │
│  ┌─────────────┐  ┌─────────────────────┐ │
│  │ Connection  │  │      Cache          │ │
│  │    Pool     │  │  (Redis/Memcached)  │ │
│  └─────────────┘  └─────────────────────┘ │
└─────────────────────────────────────────┘
                    ↓
┌─────────────────────────────────────────┐
│           Database + Redundancy         │
│  ┌─────────────┐  ┌─────────────────────┐ │
│  │  Primary DB │  │     Replica DB      │ │
│  └─────────────┘  └─────────────────────┘ │
└─────────────────────────────────────────┘
```

**Result**: ✅ **Good optimisations as of now**
**Growth**: **Scaled the business to one more city, and now getting ~100 booking per minute**

### Pattern 2: Vertical Scaling or Scale-up

#### **Challenge**
```
Business is growing, you decided to scale it to 3 more cities and now getting 300 booking per minute
```

#### **Solution**
```
Vertical Scaling Approach:
• Upgrading our initial tiny machine
• RAM by 2x and SSD by 3x etc.
```

**Architecture**:
```
Before Scale-up:
┌─────────────────────────────────────────┐
│          Small Machine                  │
│         4 Cores, 8GB RAM               │
│               500GB SSD               │
└─────────────────────────────────────────┘

After Scale-up:
┌─────────────────────────────────────────┐
│          Upgraded Machine               │
│         8 Cores, 16GB RAM              │
│              1.5TB SSD                │
└─────────────────────────────────────────┘
```

**Cost Analysis**:
```
Scale-up Cost Progression:
• 2x RAM: +$200
• 3x SSD: +$300
• 2x CPU: +$400
Total: +$900 (Pocket friendly)

But...
• Further scaling: Cost increases exponentially
• 4x improvement: +$2,000
• 8x improvement: +$8,000
• 16x improvement: +$25,000
```

**Result**: ✅ **Good Optimisation as of now**
**Limitation**: **Scale up is pocket friendly till a point only. More you scale up, cost increases exponentially**

### Pattern 3: Command Query Responsibility Segregation (CQRS)

#### **Challenge**
```
Business is growing, you decided to scale it to 2 more cities
• Primary is not able to handle all write requests
• Lag between primary and replica is impacting user experience
```

#### **Solution**
```
CQRS Implementation:
• Separate read/write operations physical machine wise
• 2 more machines as replica to the primary machine
• All read queries to replicas
• All write queries to primary
```

**Architecture**:
```
Application Layer:
┌─────────────────────────────────────────┐
│              Application                │
│  ┌─────────────┐  ┌─────────────────────┐ │
│  │ Write Query │  │    Read Query       │ │
│  │    Logic    │  │      Logic          │ │
│  └─────────────┘  └─────────────────────┘ │
└─────────────────────────────────────────┘
        ↓                    ↓
┌─────────────┐    ┌─────────────────────┐
│ Primary DB  │    │    Replica DBs      │
│ (Write Only)│    │   (Read Only)       │
└─────────────┘    └─────────────────────┘
```

**Benefits**:
- **Write operations** not affected by read load
- **Read operations** distributed across replicas
- **Better resource utilization**
- **Improved query performance**

**Challenges**:
- **Replication lag** between primary and replicas
- **Data consistency** management
- **Complex architecture** to maintain

### Pattern 4: Multi Primary Replication

#### **Challenge**
```
You scale to 5 more cities & your system is in pain again (~50 req/s)
```

#### **Solution**
```
Multi Primary Configuration:
• Why not distribute write request to replica also?
• All machines can work as primary & replica
• Multi primary configuration is a logical circular ring
• Write data to any node
• Read data from any node that replies to the broadcast first
```

**Architecture**:
```
Multi-Primary Ring Architecture:
     ┌─────────────┐
     │   Node A    │ ←── Write/Read
     │ (Primary+   │
     │  Replica)   │
     └─────┬───────┘
           ↕
     ┌─────┴───────┐
     │   Node B    │ ←── Write/Read
     │ (Primary+   │
     │  Replica)   │
     └─────┬───────┘
           ↕
     ┌─────┴───────┐
     │   Node C    │ ←── Write/Read
     │ (Primary+   │
     │  Replica)   │
     └─────┬───────┘
           ↕
     (Back to Node A - Circular Ring)
```

**Data Flow**:
```
Write Operation:
Node A → Replicate to Node B → Replicate to Node C → Back to Node A

Read Operation:
Application → Any Node → Response (First to reply)
```

**Benefits**:
- **Write distribution** across multiple nodes
- **No single write bottleneck**
- **High availability** for both reads and writes
- **Geographic distribution** possible

**Challenges**:
- **Complex conflict resolution**
- **Data consistency** management
- **Network partition** handling
- **Write latency** due to replication

### Pattern 5: Partitioning of Data by Functionality

#### **Challenge**
```
Planning to expand your business to other country
```

#### **Solution**
```
Functional Partitioning:
• What about separating the location tables in separate DB schema?
• What about putting that DB in separate machines with primary-replica or multi-primary configuration?
• Different DB can host data categorised by different functionality
• Backend or application layer has to take responsibility to join the results
```

**Architecture**:
```
Functional Partitioning:
┌─────────────────────────────────────────┐
│              Application                │
│  ┌─────────────┬─────────────┬─────────┐ │
│  │   Users     │  Bookings   │Location │ │
│  │   Service   │  Service    │ Service │ │
│  └─────────────┴─────────────┴─────────┘ │
└─────────────────────────────────────────┘
        ↓             ↓             ↓
┌─────────────┐ ┌─────────────┐ ┌─────────────┐
│   User DB   │ │ Booking DB  │ │Location DB │
│             │ │             │ │             │
│ ┌─────────┐ │ │ ┌─────────┐ │ │ ┌─────────┐ │
│ │Users    │ │ │Trips    │ │ │Cities   │ │
│ │Profiles │ │ │Bookings │ │ │Areas    │ │
│ └─────────┘ │ │ └─────────┘ │ │ └─────────┘ │
└─────────────┘ └─────────────┘ └─────────────┘
```

**Implementation Example**:
```
Database Separation:
User Database:
- Users table
- User profiles
- User preferences
- Authentication data

Booking Database:
- Trips table
- Booking history
- Payment records
- Trip ratings

Location Database:
- Cities table
- Areas/neighborhoods
- Geographic data
- Location-based services
```

**Benefits**:
- **Domain separation** for better management
- **Independent scaling** per domain
- **Team specialization** possible
- **Targeted optimization** per functionality

**Challenges**:
- **Cross-functional joins** require application-level logic
- **Data consistency** across databases
- **Transaction management** complexity
- **Increased network** calls

### Pattern 6: Horizontal Scaling or Scale-out (Sharding)

#### **Challenge**
```
Scaling the business across continents
```

#### **Solution**
```
Sharding Implementation:
• Sharding - multiple shards
• Allocate 50 machines - all having same DB schema
• Each machine just hold a part of data
• Locality of data should be there
• Each machine can have their own replicas, may be used in failure recovery
```

**Architecture**:
```
Sharding Architecture:
                    ┌─────────────────────────────────────┐
                    │              Application            │
                    │        ┌─────────────────────┐       │
                    │        │    Routing Layer     │       │
                    │        │ (Shard Key Router)   │       │
                    │        └─────────────────────┘       │
                    └─────────────────────────────────────┘
                                ↓
        ┌───────────┬───────────┬───────────┬───────────┐
        │  Shard 1  │  Shard 2  │  Shard 3  │  Shard 4  │
        │ (Users    │ (Users    │ (Users    │ (Users    │
        │ 1-25M)    │ 25M-50M)  │ 50M-75M)  │ 75M-100M) │
        └───────────┴───────────┴───────────┴───────────┘
        ↓           ↓           ↓           ↓
    ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌─────────┐
    │ Replica │ │ Replica │ │ Replica │ │ Replica │
    │  Set A  │ │  Set B  │ │  Set C  │ │  Set D  │
    └─────────┘ └─────────┘ └─────────┘ └─────────┘
```

**Sharding Strategy Example**:
```
User ID-based Sharding:
Shard 1: User IDs 1 - 25,000,000 (North America)
Shard 2: User IDs 25,000,001 - 50,000,000 (Europe)
Shard 3: User IDs 50,000,001 - 75,000,000 (Asia)
Shard 4: User IDs 75,000,001 - 100,000,000 (Rest of World)

Geographic Sharding:
Shard 1: US-based users
Shard 2: Europe-based users
Shard 3: Asia-based users
Shard 4: Other regions
```

**Benefits**:
- **Unlimited horizontal scaling**
- **Geographic distribution** for low latency
- **Independent scaling** per shard
- **Fault isolation** between shards

**Challenges**:
- **Sharding is generally hard to apply**
- **Complex routing logic** required
- **Re-sharding complexity** when data grows
- **Cross-shard queries** difficult

### Pattern 7: Data Centre Wise Partition

#### **Challenge**
```
Scaling the business across continents
Requests travelling across continents are having high latency
```

#### **Solution**
```
Data Centre Distribution:
• What about distributing traffic across data centres?
• Data centres across continents
• Enable cross data centre replication which helps disaster recovery
• This always maintain Availability of your system
```

**Architecture**:
```
Global Data Centre Architecture:
┌─────────────────────────────────────────────────────────────┐
│                      Global Load Balancer                   │
└─────────────────────────────────────────────────────────────┘
                                ↓
        ┌───────────────────────┼───────────────────────┐
        ↓                       ↓                       ↓
┌─────────────────┐   ┌─────────────────┐   ┌─────────────────┐
│  US Data Centre │   │ Europe Data     │   │ Asia Data       │
│                 │   │ Centre          │   │ Centre          │
│ ┌─────────────┐ │   │ ┌─────────────┐ │   │ ┌─────────────┐ │
│ │ Shard A     │ │   │ │ Shard D     │ │   │ │ Shard G     │ │
│ │ Shard B     │ │   │ │ Shard E     │ │   │ │ Shard H     │ │
│ │ Shard C     │ │   │ │ Shard F     │ │   │ │ Shard I     │ │
│ └─────────────┘ │   │ └─────────────┘ │   │ └─────────────┘ │
│                 │   │                 │   │                 │
│ ┌─────────────┐ │   │ ┌─────────────┐ │   │ ┌─────────────┐ │
│ │ Replication │ │   │ │ Replication │ │   │ │ Replication │ │
│ │   Layer     │ │   │ │   Layer     │ │   │ │   Layer     │ │
│ └─────────────┘ │   │ └─────────────┘ │   │ └─────────────┘ │
└─────────────────┘   └─────────────────┘   └─────────────────┘
        └─────────────────────── Cross-Data Centre Replication ────────────────────────┘
```

**Geographic Routing**:
```
User Request Flow:
User from US → US Data Centre (Low latency)
User from UK → Europe Data Centre (Low latency)
User from Japan → Asia Data Centre (Low latency)

Cross-Region Data Sync:
US Data Centre ↔ Europe Data Centre ↔ Asia Data Centre
(Async replication for disaster recovery)
```

**Benefits**:
- **Low latency** for regional users
- **High availability** across continents
- **Disaster recovery** capabilities
- **Regulatory compliance** (data residency)
- **Load distribution** globally

**Final Goal**: **Now, Plan for an IPO :p**

---

## Interview Questions 🎯

### Q1: What is the difference between partitioning and sharding?
**Answer**:
**Partitioning**:
- **Database feature** for splitting tables within a single database
- **Transparent to applications** - queries work normally
- **Limited scalability** within single database
- **Lower complexity** in implementation

**Sharding**:
- **Application-level design** for splitting across multiple databases
- **Requires routing logic** to direct queries to correct shard
- **Unlimited scalability** across multiple servers
- **Higher complexity** in implementation and maintenance

### Q2: When should you apply database partitioning?
**Answer**:
**Apply partitioning when:**
- **Dataset becomes huge** (>10GB or >10M rows) making management tedious
- **Number of requests large** causing high response times
- **Single DB server access** taking huge time
- **System response time becomes unacceptably high**
- **Backup/restore operations** becoming too slow
- **Index maintenance** consuming significant resources

### Q3: Explain the advantages of database partitioning
**Answer**:
**Five key advantages:**
1. **Parallelism**: Multiple partitions queried simultaneously for faster execution
2. **Availability**: If one partition fails, others remain accessible
3. **Performance**: Smaller partitions enable faster queries and maintenance
4. **Manageability**: Easier backup, restore, and index operations on smaller datasets
5. **Cost Reduction**: Horizontal scaling cheaper than expensive vertical scaling

### Q4: What is vertical partitioning and when would you use it?
**Answer**:
**Vertical partitioning** slices tables column-wise across different servers.

**Use cases:**
- **Wide tables** with many columns where queries only need subsets
- **Security requirements** where certain columns need separate access
- **Performance optimization** for frequently accessed columns
- **Archival scenarios** where historical data separated from current data

**Example**: User table split into basic info (ID, name, email) and detailed info (profile, preferences, settings).

### Q5: Explain the scatter-gather problem in sharding
**Answer**:
**Scatter-gather problem** occurs when analytical queries need data from multiple shards.

**Process**:
1. **Scatter**: Query sent to all shards containing relevant data
2. **Gather**: Results collected and aggregated from all shards
3. **Combine**: Final result assembled from partial results

**Issues**:
- **High latency** due to multiple network calls
- **Complex aggregation** logic required
- **Performance degradation** compared to single database
- **Limited analytical capabilities** across shards

**Example**: "SELECT COUNT(*) FROM users WHERE country IN ('USA', 'UK', 'India')" requires querying all shards that might contain users from these countries.

### Q6: Describe the evolution from single database to distributed system using the cab booking case study
**Answer**:
**Evolution stages:**
1. **Single machine** (~10 customers, 1 booking/5min)
2. **Query optimization + connection pooling** (30 bookings/min)
3. **Vertical scaling** (100 bookings/min)
4. **CQRS** - separate read/write servers (300 bookings/min)
5. **Multi-primary replication** (50 req/s across multiple cities)
6. **Functional partitioning** (different databases per domain)
7. **Sharding** (multiple machines with same schema)
8. **Data center distribution** (global deployment across continents)

**Key insight**: Each pattern solves specific problems as scale increases, with increasing complexity and cost.

### Q7: What is CQRS and what problems does it solve?
**Answer**:
**CQRS (Command Query Responsibility Segregation)** separates read and write operations at the database level.

**Implementation**:
- **Write operations** go to primary database
- **Read operations** go to replica databases
- **Separation** at physical machine level

**Problems solved**:
- **Write bottlenecks** separated from read load
- **Read performance** optimized independently
- **Better resource utilization** for different operation types
- **Scaling independence** for reads vs writes

**Trade-offs**:
- **Replication lag** between primary and replicas
- **Eventual consistency** for read operations
- **Increased architecture complexity**

### Q8: What are the challenges of implementing sharding?
**Answer**:
**Major challenges:**
1. **Complexity**: Requires partition mapping and routing layer implementation
2. **Re-sharding**: Non-uniform data distribution creates need for re-balancing
3. **Cross-shard queries**: Scatter-gather problem for analytical queries
4. **Data consistency**: Maintaining consistency across shards is difficult
5. **Operational complexity**: More difficult to manage multiple databases
6. **Transaction management**: Cross-shard transactions are complex

**Best practice**: "No Pain, No Gain" - sharding is hard to apply but necessary for massive scale.

---

## Quick Reference Table 📋

| **Scaling Pattern** | **When to Apply** | **Key Benefit** | **Complexity** |
|--------------------|-------------------|-----------------|----------------|
| **Query Optimization** | < 100 requests/min | Quick performance wins | Low |
| **Vertical Scaling** | 100-300 requests/min | Simple upgrade | Low |
| **CQRS** | 300-1000 requests/min | Separate read/write | Medium |
| **Multi-Primary** | Multiple cities | Write distribution | Medium |
| **Functional Partitioning** | Multiple domains | Domain separation | High |
| **Sharding** | Massive scale | Unlimited scaling | Very High |
| **Data Center Distribution** | Global deployment | Low latency globally | Very High |

| **Technique** | **Partitioning Type** | **Use Case** | **Challenge** |
|---------------|---------------------|--------------|---------------|
| **Vertical Partitioning** | Column-wise | Wide tables, security | Cross-server joins |
| **Horizontal Partitioning** | Row-wise | Large datasets | Partitioning strategy |
| **Sharding** | Across databases | Massive scale | Routing complexity |
| **Functional Partitioning** | By domain | Multi-domain apps | Cross-domain joins |
| **Geographic Partitioning** | By location | Global applications | Cross-region latency |

---

## Key Takeaways 💡

1. **Partitioning divides large databases** into smaller, manageable pieces
2. **Vertical partitioning** splits columns, **horizontal partitioning** splits rows
3. **Sharding is advanced horizontal partitioning** across multiple databases
4. **Apply partitioning when** data becomes huge or performance degrades
5. **Five main benefits**: parallelism, availability, performance, manageability, cost reduction
6. **Database scaling evolves** through patterns from simple optimization to distributed systems
7. **Each scaling pattern** solves specific problems with increasing complexity
8. **Distributed databases** are the product of applying multiple optimization techniques
9. **Sharding provides unlimited scaling** but introduces significant complexity
10. **Global applications** require data center distribution for optimal performance

**Remember**: Database optimization is a journey - start with simple optimizations and progressively apply more complex patterns as your application scales! 🚀