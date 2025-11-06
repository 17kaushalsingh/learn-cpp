# Chapter 10: Normalization 📋

## What is Normalization? 🤔

**Normalization is a step towards DB optimization** - a systematic approach to organize data in a database efficiently.

Think of normalization like **organizing your messy room**. Instead of throwing everything in one big box, you put similar items together in separate, organized boxes.

**Normalization = Organizing data to avoid confusion and duplication**

---

## Functional Dependency (FD) 🔗

### Definition
**Functional Dependency is a relationship between the primary key attribute (usually) of the relation to that of the other attribute of the relation.**

**Notation**: `X → Y`
- **X** = **Determinant** (left side of FD)
- **Y** = **Dependent** (right side of FD)

### Simple Definition
**FD tells us: "If I know X, I can figure out Y"**

### Real-Life Examples:
- `Student_ID → Student_Name` (Know student ID → Know name)
- `ISBN → Book_Title` (Know ISBN → Know book title)
- `Email → User_Name` (Know email → Know username)

### Types of Functional Dependencies

#### 1. Trivial FD 🥱
**A → B has trivial functional dependency if B is a subset of A. A→A, B→B are also Trivial FD.**

**Examples**:
- `A → A`
- `B → B`
- `{Student_ID, Name} → Name` (since Name ⊆ {Student_ID, Name})

**Why it's "trivial"**: If you have both ID and name, you obviously know the name!

#### 2. Non-Trivial FD 🚀
**A → B has a non-trivial functional dependency if B is not a subset of A. [A intersection B is NULL].**

**Example**:
- `Student_ID → Student_Name` (Student_Name ∉ {Student_ID})

**Why it's "non-trivial"**: Knowing ID gives you NEW information (name)

---

## Armstrong's Axioms (Rules of FD) 🛠️

These rules help us create new functional dependencies from existing ones.

### 1. Reflexive Rule 🔄
**If 'A' is a set of attributes and 'B' is a subset of 'A'. Then, A→B holds.**
**If A ⊇ B then A → B.**

**Example**:
- `{Student_ID, Name} → Student_ID`
- `{A, B, C} → A` (since A ⊆ {A, B, C})

### 2. Augmentation Rule ➕
**If B can be determined from A, then adding an attribute to this functional dependency won't change anything.**
**If A→B holds, then AX→BX holds too. 'X' being a set of attributes.**

**Example**:
- If `Student_ID → Name`
- Then `{Student_ID, Course} → {Name, Course}`

### 3. Transitivity Rule 🔗
**If A determines B and B determines C, we can say that A determines C.**
**If A→B and B→C then A→C.**

**Example**:
- `Student_ID → Dept_ID`
- `Dept_ID → Dept_Name`
- **Therefore**: `Student_ID → Dept_Name`

---

## Why Do We Need Normalization? 🎯

### Main Goal
**To avoid redundancy in the DB, not to store redundant data.**

### The Problem: Data Redundancy
Imagine you have this table:

| Student_ID | Name | Course | Professor | Professor_Email |
|------------|------|--------|-----------|-----------------|
| 101        | John | Math   | Dr. Smith | smith@college   |
| 101        | John | Physics| Dr. Smith | smith@college   |
| 102        | Jane | Math   | Dr. Smith | smith@college   |

**Problems here:**
1. **Redundancy**: Dr. Smith's email repeated 3 times
2. **Update Mess**: If Dr. Smith changes email, you must update 3 places
3. **Delete Risk**: If John leaves, you might lose course information
4. **Insert Issue**: Can't add a new course without a student

---

## Database Anomalies 😱

**Anomalies means abnormalities, there are three types of anomalies introduced by data redundancy.**

### 1. Insertion Anomaly 🚫
**When certain data (attribute) cannot be inserted into the DB without the presence of other data.**

**Problem**: Can't add a new course until a student takes it
**Real Example**: Can't add a new professor until they teach a course
**Solution**: Separate course table and professor table

### 2. Deletion Anomaly 💀
**The delete anomaly refers to the situation where the deletion of data results in the unintended loss of some other important data.**

**Problem**: Removing last student from a course deletes course info
**Real Example**: If only one student takes a course and that student leaves, you lose the course information
**Solution**: Keep course data separate from enrollment data

### 3. Updation Anomaly (or Modification Anomaly) 🔄
**The update anomaly is when an update of a single data value requires multiple rows of data to be updated.**
**Due to updation to many places, may be Data inconsistency arises, if one forgets to update the data at all the intended places.**

**Problem**: Professor's email change requires updating many rows
**Real Example**: Professor gets married and changes name - must update in every course they teach
**Risk**: If you miss updating one row, data becomes inconsistent
**Solution**: One place for professor data

### Impact on Database Performance
**Due to these anomalies, DB size increases and DB performance becomes very slow.**

**Solution**: To rectify these anomalies and the effects on DB, we use Database optimization technique called **NORMALIZATION**.

---

## What is Normalization? 📋

**Normalization is used to minimise the redundancy from relations. It is also used to eliminate undesirable characteristics like Insertion, Update, and Deletion Anomalies.**

**Normalization divides the composite attributes into individual attributes OR larger table into smaller and links them using relationships.**

**The normal form is used to reduce redundancy from the database table.**

---

## Types of Normal Forms 📊

Think of normal forms like **levels of cleanliness** for your room.

### 1NF: First Normal Form 🧹
**Requirements**:
1. **Every relation cell must have atomic value**
2. **Relation must not have multi-valued attributes**

**Before 1NF (Messy)**:
```
Student: 101
Name: John
Courses: [Math, Physics, Chemistry]  ❌ Multiple values
```

**After 1NF (Clean)**:
```
101 | John | Math
101 | John | Physics
101 | John | Chemistry ✅ One value per cell
```

### 2NF: Second Normal Form 🧹🧹
**Requirements**:
1. **Relation must be in 1NF**
2. **There should not be any partial dependency**
   - **All non-prime attributes must be fully dependent on PK**
   - **Non-prime attribute cannot depend on the part of the PK**

**What is Partial Dependency?**
When non-key data depends on only PART of the composite primary key.

**Example Problem**:
```
Table: {Student_ID, Course_ID} → Student_Name, Grade, Course_Name
Key: {Student_ID, Course_ID}

Student_Name depends only on Student_ID (part of key) ❌ Partial Dependency
Course_Name depends only on Course_ID (part of key) ❌ Partial Dependency
Grade depends on both Student_ID and Course_ID ✅ Full Dependency
```

**Solution (2NF)**: Break into separate tables:
```
Students: {Student_ID} → Student_Name
Courses: {Course_ID} → Course_Name
Grades: {Student_ID, Course_ID} → Grade
```

### 3NF: Third Normal Form 🧹🧹🧹
**Requirements**:
1. **Relation must be in 2NF**
2. **No transitivity dependency exists**
   - **Non-prime attribute should not determine a non-prime attribute**

**What is Transitive Dependency?**
A → B → C (where A is key, B and C are non-key)

**Example Problem**:
```
Table: Student_ID → Student_Name, Dept_ID, Dept_Name
Key: Student_ID

Student_ID → Dept_ID ✅ (Valid)
Dept_ID → Dept_Name ❌ (Transitive Dependency - non-prime determining non-prime)
```

**Solution (3NF)**: Break it:
```
Students: {Student_ID} → Student_Name, Dept_ID
Departments: {Dept_ID} → Dept_Name
```

### 4. BCNF (Boyce-Codd Normal Form) 🏆
**Requirements**:
1. **Relation must be in 3NF**
2. **For FD: A → B, A must be a super key**
   - **We must not derive prime attribute from any prime or non-prime attribute**

**What does this mean?**
If A → B, then A must be able to determine ALL attributes in the table

**Example Problem**:
```
Table: {Student, Course} → Professor
Professor → Course

Key: {Student, Course}

Professor → Course violates BCNF because:
Professor is NOT a super key (doesn't determine Student)
```

**Solution (BCNF)**:
```
Table1: {Student, Course} → Professor
Table2: {Professor} → Course
```

---

## Advantages of Normalization ✨

### 1. **Normalization helps to minimise data redundancy**
- Eliminates duplicate data storage
- Reduces storage requirements
- Each piece of data stored only once

### 2. **Greater overall database organisation**
- Structured and logical data arrangement
- Clear relationships between entities
- Easier to understand and maintain

### 3. **Data consistency is maintained in DB**
- Reduces update anomalies
- Ensures accurate and reliable data
- Eliminates data inconsistency issues

---

## Step-by-Step Example 🎯

### Original Messy Table:
```
Library_Book:
Book_ID | Title | Author_Name | Author_Email | Category | Category_Desc
1       | SQL   | John        | john@email   | DB       | Database
2       | C++   | John        | john@email   | PL       | Programming
3       | Java  | Mary        | mary@email   | PL       | Programming
```

### Problems Found:
1. **Redundancy**: John's info repeated, Programming category repeated
2. **Update**: John changes email → update multiple rows
3. **Insert**: Can't add new author without book
4. **Delete**: Remove all John's books → lose author info

### Step 1: Check 1NF ✅
All cells have single values → Good

### Step 2: Find Dependencies:
```
Book_ID → Title, Author_Name, Author_Email, Category, Category_Desc
Author_Name → Author_Email
Category → Category_Desc
```

### Step 3: Check 2NF
**Primary Key**: Book_ID
**No partial dependencies** (only one attribute in key) → Good

### Step 4: Check 3NF
**Transitive dependencies found**:
```
Book_ID → Author_Name → Author_Email
Book_ID → Category → Category_Desc
```

### Step 5: Normalize to 3NF:
```
Books: {Book_ID} → Title, Author_Name, Category
Authors: {Author_Name} → Author_Email
Categories: {Category} → Category_Desc
```

**Final Result**: Clean, organized, no redundancy! 🎉

---

## Quick Decision Tree 🌳

```
Start: Is your table in 1NF?
├── No: Make atomic values
└── Yes: Go to 2NF
    ├── No: Remove partial dependencies
    └── Yes: Go to 3NF
        ├── No: Remove transitive dependencies
        └── Yes: Go to BCNF
            ├── No: Make every determinant a super key
            └── Yes: Perfectly normalized! 🏆
```

---

## Interview Questions (Simplified) 💼

### Q1: What's the difference between 3NF and BCNF?
**Simple Answer**:
- **3NF**: Removes A→B→C problems (non-prime → non-prime)
- **BCNF**: Even stricter - if A→B, then A must be a super key
- **Example**:
  - 3NF allows: {Student, Course} → Professor, Professor → Office
  - BCNF says: No! Professor must be a super key

### Q2: Should I always normalize to BCNF?
**Simple Answer**:
**No!** Sometimes you denormalize for performance.
- **Normalize**: When data integrity is crucial (banking, user data)
- **Denormalize**: When read speed is crucial (analytics, reporting)

### Q3: Find normal form: R(A,B,C,D) with FDs: {A→B, C→D}
**Step-by-Step**:
1. **Key**: {A, C} (only combination that determines everything)
2. **1NF**: Assume atomic values ✅
3. **2NF**:
   - A→B is partial dependency (A is part of key)
   - **Not in 2NF**
4. **Normalize to 2NF**:
   ```
   R1(A, B) with A→B
   R2(A, C, D) with AC→AD and C→D
   ```
5. **Check R2 for 3NF**:
   - C→D is transitive dependency (non-key → non-key)
   - **Not in 3NF**
6. **Final Normalized**:
   ```
   R1(A, B) with A→B
   R2(A, C) with AC→AC
   R3(C, D) with C→D
   ```

---

## Quick Cheat Sheet 📋

| Normal Form | Main Rule | Fixes |
|-------------|-----------|-------|
| **1NF** | One value per cell | Repeating groups |
| **2NF** | No partial dependencies | Part-key dependencies |
| **3NF** | No transitive dependencies | Non-key → non-key |
| **BCNF** | Every determinant is super key | Remaining redundancy |

**Key Terms to Remember**:
- **Prime Attribute**: Part of any candidate key
- **Non-Prime Attribute**: Not part of any candidate key
- **Super Key**: Can determine ALL attributes
- **Candidate Key**: Minimal super key
- **Determinant**: Left side of FD (X in X→Y)
- **Dependent**: Right side of FD (Y in X→Y)

---

## Real-World Tips 🌟

### When to Normalize:
- **OLTP Systems** (Online Transaction Processing)
- **Banking, E-commerce, User Management**
- **Data integrity is more important than speed**

### When to Denormalize:
- **OLAP Systems** (Online Analytical Processing)
- **Data Warehousing, Reporting, Analytics**
- **Read speed is more important than storage**

### Practical Approach:
1. **Normalize first** for data integrity
2. **Denormalize selectively** for performance bottlenecks
3. **Document your decisions** for future maintenance

---

**Remember**: Normalization is about finding the **right balance** between data integrity and performance. Start normalized, then optimize as needed! 🎯

**Key Takeaway**: Normalization is a **step towards DB optimization** that helps minimize redundancy and eliminate anomalies while maintaining data consistency.