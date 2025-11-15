# Maths & Statistics Comprehensive Revision - AI Scientist Interview Prep

## Interview Overview
- **Weightage**: 33.33% (Critical Section!)
- **Time**: 30 minutes for Round 1
- **Focus**: Deep conceptual understanding with practical applications
- **What Interviewers Want**: Clear reasoning, step-by-step approach, intuition behind formulas

## Table of Contents
1. [Probability](#probability)
2. [Probability Distributions](#probability-distributions)
3. [Sampling](#sampling)
4. [Correlations](#correlations)
5. [Hypothesis Testing](#hypothesis-testing)
6. [Statistical Significance](#statistical-significance)
7. [Central Limit Theorem](#central-limit-theorem)
8. [Paired Means Tests](#paired-means-tests)
9. [Bayes' Theorem](#bayes-theorem)
10. [Linear Algebra](#linear-algebra)

---

## Probability

### Fundamental Concepts (Must Know for Interview)

#### 1. Basic Probability Definition
**Intuition**: Probability measures how likely something is to happen, ranging from 0 (impossible) to 1 (certain)

**Mathematical Definition**:
P(A) = Number of favorable outcomes ÷ Total possible outcomes

**Why This Matters**: This is the foundation of all statistical thinking in ML and data science.

#### 2. Sample Space and Events
```
Sample Space (S): All possible outcomes
Event (A): Subset of sample space
Complement (A'): All outcomes NOT in A
```

**Example**: Rolling a die
- Sample Space S = {1, 2, 3, 4, 5, 6}
- Event A = "even number" = {2, 4, 6}
- Complement A' = "odd number" = {1, 3, 5}

#### 3. Conditional Probability - The Heart of Many ML Algorithms

**Intuition**: "What's the probability of A happening, given that B has already happened?"

**Formula**: P(A|B) = P(A ∩ B) / P(B)

**Step-by-Step Thought Process**:
1. What's the new "world" we're living in? (B has happened)
2. Within this new world, what fraction corresponds to A?
3. This becomes our conditional probability

**Why Crucial**: Naive Bayes, Bayesian networks, medical diagnosis, spam filters all depend on this!

#### 4. Independence - A Special Case

**Intuition**: Two events don't influence each other at all

**Mathematical Condition**: P(A|B) = P(A) OR P(B|A) = P(B)
**Practical Formula**: P(A ∩ B) = P(A) × P(B)

**Common Mistake**: Assuming independence when it's not true!

### Key Probability Rules with Intuition

#### 1. Addition Rule (OR scenarios)
**When to Use**: "What's probability of A OR B happening?"

```
General Case: P(A ∪ B) = P(A) + P(B) - P(A ∩ B)
Special Case (Mutually Exclusive): P(A ∪ B) = P(A) + P(B)
```

**Intuition**: Add probabilities, but subtract overlap to avoid double-counting!

**Interview Tip**: Always check if events are mutually exclusive first!

#### 2. Multiplication Rule (AND scenarios)
**When to Use**: "What's probability of A AND B both happening?"

```
General Case: P(A ∩ B) = P(A) × P(B|A)
Special Case (Independent): P(A ∩ B) = P(A) × P(B)
```

**Intuition**: First probability of A, then probability of B given A has happened

### Deep Dive Interview Questions

#### Q1: Two Dice Problem (Foundation Level)
**Question**: Two fair dice are rolled. What's the probability that the sum is exactly 7?

**Step-by-Step Solution**:
1. **Sample Space**: 6 × 6 = 36 total outcomes
2. **Favorable Outcomes**: (1,6), (2,5), (3,4), (4,3), (5,2), (6,1) = 6 outcomes
3. **Probability**: 6/36 = 1/6

**Interview Follow-ups**:
- What about sum > 7? (15 favorable outcomes → 15/36 = 5/12)
- What about sum is prime? (2,3,5,7,11 → count these)
- What if dice are biased? How would approach change?

#### Q2: Card Drawing Without Replacement (Tests Conditional Probability)
**Question**: From a standard deck, you draw 2 cards without replacement. What's P(both are hearts)?

**Intuitive Approach**:
1. **First draw**: P(heart) = 13/52 = 1/4
2. **Second draw**: Given first was heart, now 12 hearts left, 51 cards total
3. **Second draw**: P(heart|first was heart) = 12/51
4. **Both events**: P = (13/52) × (12/51) = 1/17

**Alternative Solution Using Combinations**:
- Total ways to choose 2 cards: C(52,2) = 1326
- Ways to choose 2 hearts: C(13,2) = 78
- Probability: 78/1326 = 1/17

**Interview Tip**: Show both methods - demonstrates versatility!

#### Q3: Birthday Problem (Classic Interview Question)
**Question**: In a room of 23 people, what's the probability that at least 2 people share the same birthday?

**Intuitive Strategy**: Calculate the opposite probability (NO shared birthdays), then subtract from 1.

**Step-by-Step Solution**:
1. **Person 1**: Can have any birthday (365/365)
2. **Person 2**: Must have different birthday (364/365)
3. **Person 3**: Must avoid previous 2 birthdays (363/365)
4. **Continue**... Person 23: (343/365)
5. **Multiply all together**: (365×364×363×...×343)/365²³
6. **Result**: ≈ 0.4927 (49.27% chance NO shared birthdays)
7. **Final Answer**: 1 - 0.4927 = 0.5073 (50.73% chance at least 2 share birthday)

**Why This Interview Question**:
- Tests understanding of complementary probability
- Shows how small individual probabilities multiply to significant results
- Demonstrates counterintuitive nature of probability

**Follow-up Questions**:
- How many people needed for 90% probability? (≈ 41 people)
- What about leap years? How does this affect the calculation?
- How would you estimate this without exact calculation?

#### Q4: Medical Testing (Critical for Data Science Applications)
**Question**: A disease affects 1% of population. A test is 99% accurate. If you test positive, what's the probability you actually have the disease?

**This is a Bayes' Theorem problem in disguise! Let's solve intuitively:**

**Step 1: Assume 10,000 people (easy numbers)**
- **Actually sick**: 100 people (1% of 10,000)
- **Actually healthy**: 9,900 people

**Step 2: Test results**
- **True positives**: 99 sick people × 99% accuracy = 99 people
- **False positives**: 9,900 healthy × 1% error rate = 99 people
- **Total positive tests**: 99 + 99 = 198 people

**Step 3: Final probability**
- **Actually sick given positive test**: 99/198 = 50%

**Why This is Shocking**: Only 50% chance despite 99% accurate test!

**Interview Insight**: This demonstrates why base rates are crucial in ML and data science.

#### Q5: Monte Hall Problem (Tests Conditional Thinking)
**Question**: You're on a game show. 3 doors, 1 car behind 1 door. You pick door 1. Host opens door 3 showing goat. Should you switch to door 2?

**Intuitive Solution**:
```
Initial choice: Door 1 (1/3 probability)
Switching strategy wins if car was behind doors 2 or 3
- Car behind door 2: Host opens door 3, you switch → WIN
- Car behind door 3: Host opens door 2, you switch → WIN
- Car behind door 1: Host opens either door, you switch → LOSE

Switching wins 2/3 of the time!
```

**Key Insight**: Host's action provides information that changes probabilities!

### Advanced Probability Concepts

#### 1. Expected Value (Foundation of ML)
**Intuition**: Long-term average if you repeat experiment many times

**Formula**: E[X] = Σ(x × P(x))

**Example**: Roll a fair die, win amount shown
E = 1×(1/6) + 2×(1/6) + 3×(1/6) + 4×(1/6) + 5×(1/6) + 6×(1/6) = 3.5

**Why Important**: Decision theory, reinforcement learning, expected utility

#### 2. Variance and Standard Deviation
**Intuition**: How spread out are the values?

**Formula**: Var(X) = E[(X - μ)²] = E[X²] - (E[X])²
**Standard Deviation**: σ = √Var(X)

#### 3. Covariance and Correlation Preview
```
Cov(X,Y) = E[(X - μₓ)(Y - μᵧ)]
Correlation = Cov(X,Y) / (σₓ × σᵧ)
```

### Interview Strategy for Probability Questions

#### What Interviewers Look For:
1. **Clear problem restatement**: "Let me understand what we're trying to find..."
2. **Step-by-step approach**: Break down complex problems
3. **Multiple solution methods**: Show flexibility in thinking
4. **Intuition behind formulas**: Not just memorization
5. **Connection to real applications**: How does this apply to ML/data science?

#### Common Traps to Avoid:
❌ Assuming independence without checking
❌ Confusing P(A|B) with P(B|A)
❌ Forgetting to consider all possible cases
❌ Making arithmetic errors under pressure
❌ Not explaining your thought process

#### Pro Tips:
✅ Always verify your answer makes sense (0 ≤ probability ≤ 1)
✅ Use complementary probability when it's easier
✅ Draw diagrams or tables when helpful
✅ Practice mental math for common fractions
✅ Remember key probabilities: 1/2, 1/3, 1/4, 1/6, 1/8

### Real Interview Scenarios

#### Scenario 1: A/B Testing
**Question**: "We ran an A/B test. Version A got 100 conversions from 1000 users, Version B got 120 from 1000. Is this statistically significant?"

**Approach**: This leads to hypothesis testing, but starts with probability concepts!

#### Scenario 2: ML Model Evaluation
**Question**: "Our spam filter has 95% accuracy. Only 2% of emails are spam. What's P(email is spam | flagged as spam)?"

**Connection**: Direct application of Bayes' Theorem!

#### Scenario 3: Quality Control
**Question**: "A factory produces items with 1% defect rate. We test 5 items. What's P(at least one defect)?"

**Strategy**: Use complement: 1 - P(no defects in all 5)

### Quick Reference Cheat Sheet

| Formula | When to Use | Key Insight |
|---------|-------------|-------------|
| P(A|B) = P(A∩B)/P(B) | Conditional probability | Updates knowledge |
| P(A∩B) = P(A)×P(B) | Independent events | Events don't affect each other |
| P(A∪B) = P(A)+P(B)-P(A∩B) | OR scenarios | Avoid double counting |
| E[X] = Σ(x×P(x)) | Expected value | Long-term average |
| 1 - P(no success) | At least one success | Often easier calculation |

---

## Probability Distributions

### Why Probability Distributions Matter in AI/ML

**Core Insight**: Most ML algorithms assume or learn probability distributions. Understanding them helps you:
- Choose the right model for your data
- Understand model limitations
- Debug when things go wrong
- Optimize hyperparameters

### Two Main Categories

#### 1. Discrete Distributions (Countable Outcomes)
**When to Use**: When you can count possible outcomes
**Examples**: Number of customers, defects, emails, etc.

#### 2. Continuous Distributions (Infinite Outcomes)
**When to Use**: When outcomes are measurements
**Examples**: Height, weight, time, temperature, etc.

### Deep Dive: Discrete Distributions

#### 1. Bernoulli Distribution - The Building Block
**Intuition**: Single trial with success/failure outcome

**Mathematical Definition**:
```
P(X = 1) = p (success)
P(X = 0) = 1 - p (failure)
```

**Key Properties**:
- **Mean**: E[X] = p
- **Variance**: Var(X) = p(1 - p)
- **PMF**: P(X = k) = pᵏ(1-p)¹⁻ᵏ for k ∈ {0,1}

**Real-World Applications**:
- Email spam/not spam classification
- Medical test positive/negative
- Customer purchase/no purchase

**Interview Question**: "A coin has P(heads) = 0.6. What's the expected number of heads in 3 flips?"
**Answer**: Each flip is Bernoulli(0.6), so E[total] = 3 × 0.6 = 1.8

#### 2. Binomial Distribution - Multiple Bernoulli Trials
**Intuition**: Counting successes in fixed number of independent trials

**Mathematical Definition**:
```
X ~ Bin(n, p) where:
n = number of trials
p = probability of success
X = number of successes
```

**Probability Mass Function**:
P(X = k) = C(n, k) × pᵏ × (1-p)ⁿ⁻ᵏ

Where C(n,k) = n! / (k! × (n-k)!) is the combination formula

**Step-by-Step Intuition**:
1. Choose which k trials are successes: C(n,k) ways
2. Each of those k successes happens with probability pᵏ
3. Each of the (n-k) failures happens with probability (1-p)ⁿ⁻ᵏ
4. Multiply all together!

**Key Properties**:
- **Mean**: E[X] = n × p
- **Variance**: Var(X) = n × p × (1-p)
- **Shape**: Symmetric when p=0.5, skewed otherwise

**Real-World Applications**:
- Quality control: defective items in batch
- Clinical trials: patients who respond to treatment
- Marketing: customers who click on ad

#### Binomial Interview Questions

##### Q1: Email Campaign Analysis
**Question**: An email campaign has 5% conversion rate. You send to 200 people. What's P(exactly 10 conversions)?

**Step-by-Step Solution**:
1. **Identify**: This is Binomial(n=200, p=0.05)
2. **Formula**: P(X=10) = C(200,10) × 0.05¹⁰ × 0.95¹⁹⁰
3. **Calculation**: ≈ 0.087 or 8.7%

**Interview Insight**: This tests understanding of when binomial applies!

**Follow-up Questions**:
- What's P(at least 5 conversions)? (Use 1 - P(X ≤ 4))
- What's expected number of conversions? (200 × 0.05 = 10)
- What if conversion rate is unknown? How would you estimate it?

##### Q2: Quality Control Problem
**Question**: A factory produces items with 2% defect rate. What's probability that in 50 items, at most 1 is defective?

**Solution**:
P(at most 1) = P(0) + P(1)
P(0) = C(50,0) × 0.02⁰ × 0.98⁵⁰ ≈ 0.364
P(1) = C(50,1) × 0.02¹ × 0.98⁴⁹ ≈ 0.372
Total ≈ 0.736 or 73.6%

#### 3. Poisson Distribution - Rare Events
**Intuition**: Number of events in fixed time/space interval when events are rare and independent

**Mathematical Definition**:
```
X ~ Poisson(λ) where:
λ = average rate of events
X = number of events in interval
```

**Probability Mass Function**:
P(X = k) = (λᵏ × e⁻λ) / k!

**When to Use Poisson (CRITICAL FOR INTERVIEW)**:
✅ Events are independent
✅ Average rate is constant
✅ Two events can't happen at exactly same time
✅ Events are rare relative to observation period

**Key Properties**:
- **Mean**: E[X] = λ
- **Variance**: Var(X) = λ (unique property!)
- **Shape**: Right-skewed, becomes more symmetric as λ increases

**Real-World Applications**:
- Customer arrivals per hour
- Website hits per minute
- Defects per square foot
- Calls to call center

#### Poisson Interview Questions

##### Q1: Call Center Analysis
**Question**: Call center receives 2 calls per minute on average. What's P(exactly 3 calls in next minute)?

**Solution**:
λ = 2, k = 3
P(X=3) = (2³ × e⁻²) / 3! = (8 × e⁻²) / 6 ≈ 0.180

**Follow-up**:
- P(no calls in 3 minutes)? (λ=6, P(X=0) = e⁻⁶ ≈ 0.00248)
- P(at least 1 call in 30 seconds)? (λ=1, P(X≥1) = 1 - e⁻¹ ≈ 0.632)

##### Q2: Website Traffic
**Question**: Website gets 50 hits per hour. What's P(60 or more hits in an hour)?

**Interview Strategy**:
1. **Identify**: Poisson(λ=50)
2. **Challenge**: Direct calculation of P(X≥60) requires summing many terms
3. **Practical Approach**: Use normal approximation or computational tool
4. **Key Insight**: Show understanding of approximation methods

### Deep Dive: Continuous Distributions

#### 1. Normal Distribution - The Most Important One
**Intuition**: Bell-shaped curve that appears everywhere in nature

**Mathematical Definition**:
```
X ~ N(μ, σ²) where:
μ = mean (center)
σ² = variance (spread)
σ = standard deviation
```

**Probability Density Function**:
f(x) = (1/√(2πσ²)) × e^(-(x-μ)²/(2σ²))

**Why Normal Distribution is CRUCIAL**:
1. **Central Limit Theorem**: Means of samples become normal
2. **Many natural phenomena follow it**
3. **Foundation of statistical inference**
4. **Many ML algorithms assume normality**

**Key Properties**:
- **Symmetric**: Mean = Median = Mode = μ
- **68-95-99.7 Rule**:
  - 68% within μ ± σ
  - 95% within μ ± 2σ
  - 99.7% within μ ± 3σ

**Standard Normal Transformation**:
```
Z = (X - μ) / σ ~ N(0, 1)
```

**Interview Tip**: Always standardize when working with normal distributions!

#### Normal Distribution Interview Questions

##### Q1: Height Distribution
**Question**: Male heights follow N(175, 25) cm². What % are taller than 185 cm?

**Step-by-Step Solution**:
1. **Standardize**: Z = (185-175)/√25 = 10/5 = 2
2. **Lookup**: P(Z > 2) = 1 - P(Z ≤ 2) = 1 - 0.9772 = 0.0228
3. **Answer**: 2.28% are taller than 185 cm

**Follow-up Questions**:
- What height marks the 90th percentile? (Find z for 0.90, then x = μ + zσ)
- What's P(height between 170 and 180 cm)?
- If we sample 100 men, what's distribution of average height?

##### Q2: Quality Control Tolerance
**Question**: Part lengths follow N(10, 0.04) cm². Acceptable range: 9.8 to 10.2 cm. What % of parts are acceptable?

**Solution**:
1. **Lower bound**: Z₁ = (9.8-10)/0.2 = -1
2. **Upper bound**: Z₂ = (10.2-10)/0.2 = 1
3. **Probability**: P(-1 < Z < 1) = P(Z < 1) - P(Z < -1) = 0.8413 - 0.1587 = 0.6826
4. **Answer**: 68.26% are acceptable

**Interview Insight**: Tests understanding of practical applications!

#### 2. Exponential Distribution - Waiting Times
**Intuition**: Time until next event when events happen at constant rate

**Mathematical Definition**:
```
X ~ Exp(λ) where:
λ = rate parameter (events per time unit)
X = waiting time until next event
```

**Probability Density Function**:
f(x) = λ × e^(-λx), x ≥ 0

**Cumulative Distribution Function**:
P(X ≤ x) = 1 - e^(-λx)

**Key Properties**:
- **Mean**: E[X] = 1/λ
- **Variance**: Var(X) = 1/λ²
- **Memoryless Property**: P(X > s+t | X > s) = P(X > t)

**Memoryless Property Explained**:
"The probability of waiting additional time t doesn't depend on how long you've already waited!"

**Real-World Applications**:
- Time between customer arrivals
- Time until machine failure
- Time between website hits

#### Exponential Interview Questions

##### Q1: Customer Service Analysis
**Question**: Average customer arrival rate: 10 per hour. What's P(next customer arrives within 5 minutes)?

**Solution**:
λ = 10 per hour = 10/60 per minute = 1/6 per minute
P(X ≤ 5) = 1 - e^(-λx) = 1 - e^(-(1/6)×5) = 1 - e^(-5/6) ≈ 0.565

**Follow-up**:
- What's P(no customers for 15 minutes? P(X > 15) = e^(-15/6) ≈ 0.082)
- How does this relate to Poisson? (Exponential models inter-arrival times, Poisson models count)

##### Q2: Memoryless Property Demonstration
**Question**: If you've waited 10 minutes for a bus that comes on average every 15 minutes, what's P(bus arrives in next 5 minutes)?

**Using Memoryless Property**:
P(X ≤ 15 | X > 10) = P(X ≤ 5) = 1 - e^(-5/15) = 1 - e^(-1/3)

**Interview Insight**: This demonstrates understanding of memoryless property!

### Distribution Selection Guide

| Situation | Distribution | Why? | Key Parameters |
|-----------|--------------|------|----------------|
| Single trial, success/failure | Bernoulli | Basic building block | p |
| Fixed trials, count successes | Binomial | Multiple Bernoulli | n, p |
| Rare events, fixed interval | Poisson | Events independent, rare | λ |
| Natural phenomena, measurements | Normal | CLT, common in nature | μ, σ² |
| Waiting times between events | Exponential | Constant rate, memoryless | λ |

### Interview Strategy for Distribution Questions

#### Step-by-Step Approach:
1. **Identify the scenario**: What are we measuring?
2. **Choose appropriate distribution**: Based on problem characteristics
3. **Identify parameters**: n, p, λ, μ, σ
4. **Set up equation**: Use correct PMF/PDF
5. **Calculate systematically**: Show each step
6. **Interpret results**: What does this mean in context?

#### Common Interview Mistakes:
❌ Using binomial when Poisson is appropriate
❌ Forgetting to check distribution assumptions
❌ Confusing rate λ with mean in exponential
❌ Not standardizing normal distributions
❌ Mixing up PMF and CDF

#### Pro Tips:
✅ Always state assumptions clearly
✅ Check if discrete or continuous first
✅ Remember key properties (mean, variance)
✅ Connect to real-world applications
✅ Practice mental approximations

### Advanced Topics (Bonus for Strong Candidates)

#### 1. Distribution Relationships
```
Binomial → Poisson (when n large, p small, np = λ)
Binomial → Normal (when n large, p not too close to 0 or 1)
Exponential → Gamma (sum of exponential variables)
Normal → Chi-square (sum of squared normal variables)
```

#### 2. Moment Generating Functions
**Why Important**: Uniquely characterizes distributions, useful for proving theorems

**Key Property**: If two distributions have same MGF, they are identical

#### 3. Truncated Distributions
**Real Application**: Censored data, bounded measurements

### Quick Reference Formula Sheet

#### Discrete Distributions:
```
Bernoulli: P(X=k) = pᵏ(1-p)¹⁻ᵏ, E[X]=p, Var[X]=p(1-p)
Binomial: P(X=k) = C(n,k)pᵏ(1-p)ⁿ⁻ᵏ, E[X]=np, Var[X]=np(1-p)
Poisson: P(X=k) = (λᵏe⁻λ)/k!, E[X]=λ, Var[X]=λ
```

#### Continuous Distributions:
```
Normal: f(x) = (1/√(2πσ²))e^(-(x-μ)²/(2σ²))
Standard Normal: Z = (X-μ)/σ
Exponential: f(x) = λe^(-λx), E[X]=1/λ, Var[X]=1/λ²
```

**Real Interview Question**: "A card is drawn from a standard deck. What's the probability it's a heart given that it's a face card?"

---

## Probability Distributions

### Key Distributions

**1. Discrete Distributions:**
- **Bernoulli**: Single trial with P(success) = p, P(failure) = 1-p
- **Binomial**: n independent Bernoulli trials
  - P(X = k) = C(n,k) × p^k × (1-p)^(n-k)
- **Poisson**: Number of events in fixed interval
  - P(X = k) = (λ^k × e^(-λ)) / k!

**2. Continuous Distributions:**
- **Normal**: Mean μ, variance σ²
  - f(x) = (1/√(2πσ²)) × e^(-(x-μ)²/(2σ²))
- **Exponential**: Time between events
  - f(x) = λ × e^(-λx), x ≥ 0

### Properties to Remember

| Distribution | Mean | Variance | Key Use Case |
|--------------|------|----------|-------------|
| Bernoulli | p | p(1-p) | Single success/failure |
| Binomial | np | np(1-p) | Fixed trials, success counting |
| Poisson | λ | λ | Rare event counting |
| Normal | μ | σ² | Natural phenomena |
| Exponential | 1/λ | 1/λ² | Waiting times |

### Interview Questions

#### Q1: Calls to a call center follow Poisson with λ=2/minute. What's P(no calls in 3 minutes)?
**Solution**: For 3 minutes: λ' = 2×3 = 6
P(X=0) = (6^0 × e^(-6)) / 0! = e^(-6) ≈ 0.00248

**Follow-up**: What's P(at least 2 calls)? What's expected calls in 10 minutes?

#### Q2: Heights follow N(170, 25) in cm. What % > 175cm?
**Solution**: Z = (175-170)/√25 = 5/5 = 1
P(Z > 1) = 1 - 0.8413 = 0.1587 or 15.87%

**Real Interview Question**: "A manufacturing process has 2% defect rate. What's the probability of exactly 3 defects in a batch of 100 items?"

---

## Sampling

### Key Concepts

**Sampling Methods:**
```
Simple Random Sample (SRS)
├── Each unit has equal probability
└── Selection is independent

Stratified Sampling
├── Population divided into strata
└── Random sample from each stratum

Systematic Sampling
├── Select every kth unit
└── Random starting point
```

**Sample Statistics:**
- **Sample Mean**: x̄ = Σxi / n
- **Sample Variance**: s² = Σ(xi - x̄)² / (n-1)
- **Standard Error**: SE = σ/√n

### Interview Questions

#### Q1: Population mean = 50, σ = 10. Sample size = 25. What's standard error?
**Solution**: SE = σ/√n = 10/5 = 2

**Follow-up**: What's probability sample mean > 52? How does SE change with n=100?

#### Q2: Explain stratified sampling vs simple random sampling.
**Answer**: Stratified ensures representation of all subgroups, reduces variance, better for heterogeneous populations.

**Real Interview Question**: "You have a dataset of 1M users. How would you sample 10K for analysis while ensuring representation across age groups?"

---

## Correlations

### Key Concepts

**Correlation Coefficient (r):**
- Range: [-1, 1]
- r = 1: Perfect positive linear
- r = -1: Perfect negative linear
- r = 0: No linear correlation

**Formula**: r = Σ((xi - x̄)(yi - ȳ)) / √(Σ(xi - x̄)² × Σ(yi - ȳ)²)

**Correlation Types:**
```
Pearson: Linear relationships
Spearman: Monotonic relationships (rank-based)
Kendall: Ordinal associations
```

### Interview Questions

#### Q1: Data: x=[1,2,3,4,5], y=[2,4,5,4,5]. Calculate correlation.
**Solution**: x̄=3, ȳ=4, calculate using formula → r ≈ 0.7

**Follow-up**: What if we square all x-values? How does correlation change?

#### Q2: Can correlation = 0 for nonlinearly related variables?
**Answer**: Yes! Example: y = x² for x ∈ [-2,2]. Strong nonlinear relationship but r = 0.

**Real Interview Question**: "You find correlation 0.8 between study hours and test scores. Can we conclude that more studying causes better scores?"

---

## Hypothesis Testing

### Key Concepts

**Hypothesis Structure:**
- **H₀ (Null)**: No effect/difference
- **H₁ (Alternative)**: Effect/difference exists

**Test Statistics:**
- **Z-test**: Population σ known, n ≥ 30
- **t-test**: Population σ unknown, n < 30
- **Chi-square**: Categorical data

**Decision Rules:**
- **Reject H₀**: |test statistic| > critical value
- **p-value approach**: Reject H₀ if p < α

### Common Tests

| Test | Purpose | Assumptions |
|------|---------|-------------|
| One-sample t | μ vs hypothesized value | Normality, σ unknown |
| Two-sample t | μ₁ vs μ₂ | Normality, equal variances |
| Paired t | Before/after | Paired data, normality |
| Chi-square | Goodness of fit | Expected frequencies ≥ 5 |

### Interview Questions

#### Q1: H₀: μ = 100, α = 0.05, two-tailed. Sample: n=25, x̄=105, s=10. Test hypothesis.
**Solution**: t = (105-100)/(10/√25) = 5/2 = 2.5
Critical t(24, 0.025) = ±2.064. |2.5| > 2.064 → Reject H₀

**Follow-up**: Calculate p-value. What if n=100? What's Type I/II error risk?

#### Q2: When to use t-test vs z-test?
**Answer**: t-test when σ unknown or n < 30, z-test when σ known and n ≥ 30.

**Real Interview Question**: "A/B test shows conversion rates: Control=10%, Treatment=12%, n=1000 each. Is the difference significant at α=0.05?"

---

## Statistical Significance

### Key Concepts

**p-value**: Probability of observing extreme results if H₀ is true

**Significance Level (α)**: Pre-determined threshold (usually 0.05)

**Decision Matrix:**
```
                Reality
                H₀ True    H₀ False
     Reject     Type I    Correct
     Not Reject Correct   Type II
```

**Effect Size**: Magnitude of difference, independent of sample size

### Common α Levels and Interpretation

| p-value range | Interpretation |
|---------------|----------------|
| p > 0.05 | Not significant |
| 0.01 < p ≤ 0.05 | Significant |
| 0.001 < p ≤ 0.01 | Highly significant |
| p ≤ 0.001 | Very highly significant |

### Interview Questions

#### Q1: p = 0.03 vs α = 0.01. What's the decision?
**Answer**: Do not reject H₀ (0.03 > 0.01)

**Follow-up**: What if we change α to 0.05? What's the risk of Type I error?

#### Q2: "Statistically significant but practically insignificant" - explain.
**Answer**: Large sample size can detect tiny differences that are statistically significant but have no practical importance.

**Real Interview Question**: "Your A/B test shows p=0.04 favoring version B. Should we automatically roll out version B?"

---

## Central Limit Theorem

### Key Concept

**CLT**: For any distribution with mean μ and variance σ², the sampling distribution of x̄ approaches Normal(N(μ, σ²/n)) as n → ∞

**Practical Rule**: For most distributions, CLT works well for n ≥ 30

### Mathematical Foundation

```
Sample Mean Distribution:
μ_x̄ = μ (unbiased estimator)
σ_x̄² = σ²/n (variance decreases with n)

Standard Error: SE = σ/√n
```

### Interview Questions

#### Q1: Population: Exponential with mean=2. Sample size=50. What's distribution of sample mean?
**Answer**: Approximately Normal with μ=2, σ²/50 = 4/50 = 0.08

**Follow-up**: What's P(x̄ > 2.5)? How does this change with n=100?

#### Q2: Why is CLT important in statistics?
**Answer**: Enables inference about population parameters from sample statistics, forms basis for hypothesis testing and confidence intervals.

**Real Interview Question**: "You have a skewed income distribution. Why can we still use t-tests for means with large samples?"

---

## Paired Means Tests

### Key Concept

**Paired t-test**: Compares two related samples (before/after, matched pairs)

**Test Statistic**: t = (d̄ - μ₀) / (sd/√n)

Where d̄ = mean of differences, sd = standard deviation of differences

### When to Use Paired Tests

```
✓ Same subjects measured twice
✓ Matched pairs (twins, matched groups)
✓ Before-after experiments
✓ Case-control studies
```

### Interview Questions

#### Q1: Weight before: [180,170,165], after: [175,168,160]. Test if weight loss significant.
**Solution**: Differences: [-5,-2,-5], d̄ = -4, sd = 1.73, n = 3
t = (-4-0)/(1.73/√3) = -4.01. Critical t(2,0.05) = -2.92
|t| > |critical| → Significant weight loss

**Follow-up**: Calculate 95% CI for mean difference. What assumptions are made?

#### Q2: Paired vs independent t-test - when to use which?
**Answer**: Paired for related measurements, independent for separate groups. Paired reduces variability by controlling for subject effects.

**Real Interview Question**: "You want to test if a new training program improves employee productivity. How would you design the study and which test would you use?"

---

## Bayes' Theorem

### Formula and Intuition

**Bayes' Theorem**: P(A|B) = P(B|A) × P(A) / P(B)

**Components**:
- **Prior**: P(A) - initial belief
- **Likelihood**: P(B|A) - probability of evidence given hypothesis
- **Posterior**: P(A|B) - updated belief after evidence

### Practical Form

```
P(A|B) = [P(B|A) × P(A)] / [P(B|A) × P(A) + P(B|A') × P(A')]
```

### Interview Questions

#### Q1: Disease prevalence = 1%, Test accuracy = 99%. Test positive. What's probability of having disease?
**Solution**:
P(Disease|+) = (0.99 × 0.01) / [(0.99 × 0.01) + (0.01 × 0.99)]
= 0.0099 / (0.0099 + 0.0099) = 0.5 = 50%

**Follow-up**: What if prevalence = 0.1%? What if test accuracy = 95%?

#### Q2: Two coins: one fair, one double-headed. Pick random coin, flip heads. What's probability it's double-headed?
**Solution**: P(Double|H) = (1 × 0.5) / [(1 × 0.5) + (0.5 × 0.5)] = 2/3

**Real Interview Question**: "Your ML model has 95% accuracy. In a population where 2% are fraud cases, a positive prediction occurs. What's the actual probability of fraud?"

---

## Linear Algebra

### Key Concepts

**Matrix Operations**:
- **Addition**: Element-wise
- **Multiplication**: (A×B)ᵢⱼ = Σ(Aᵢₖ × Bₖⱼ)
- **Inverse**: A⁻¹ such that A×A⁻¹ = I
- **Transpose**: (Aᵀ)ᵢⱼ = Aⱼᵢ

**Eigenvalues and Eigenvectors**:
```
A × v = λ × v
Where: v = eigenvector, λ = eigenvalue
```

**Singular Value Decomposition (SVD)**:
```
A = U × Σ × Vᵀ
U: Left singular vectors (orthogonal)
Σ: Singular values (diagonal)
Vᵀ: Right singular vectors (orthogonal)
```

### Applications in ML

| Concept | ML Application | Why Important |
|---------|----------------|----------------|
| Eigenvalues | PCA | Find principal components |
| SVD | Dimensionality reduction | Compress data efficiently |
| Matrix inverse | Linear regression | Solve normal equations |
| Matrix multiplication | Neural networks | Weight transformations |

### Interview Questions

#### Q1: Matrix A = [[2,1],[1,2]]. Find eigenvalues and eigenvectors.
**Solution**:
Characteristic equation: |A - λI| = 0
(2-λ)(2-λ) - 1 = 0 → λ² - 4λ + 3 = 0
Eigenvalues: λ₁ = 3, λ₂ = 1

For λ=3: (A-3I)v = 0 → [[-1,1],[1,-1]]v = 0 → v₁ = [1,1]
For λ=1: (A-I)v = 0 → [[1,1],[1,1]]v = 0 → v₂ = [1,-1]

**Follow-up**: What's the spectral decomposition? How does this relate to PCA?

#### Q2: Why is SVD important in machine learning?
**Answer**:
- Dimensionality reduction (keep top k singular values)
- Matrix completion/recommendation systems
- Numerical stability vs eigenvalue decomposition
- Works for any matrix (not just square)

**Real Interview Question**: "How would you use matrix decomposition to reduce the dimensionality of a user-item interaction matrix for recommendation?"

---

## Quick Reference Formulas

### Probability
- **Conditional**: P(A|B) = P(A∩B)/P(B)
- **Independence**: P(A∩B) = P(A)×P(B)
- **Total Probability**: P(A) = ΣP(A|Bᵢ)P(Bᵢ)

### Distributions
- **Binomial**: P(X=k) = C(n,k) × pᵏ × (1-p)ⁿ⁻ᵏ
- **Poisson**: P(X=k) = λᵏ × e⁻λ / k!
- **Normal**: f(x) = (1/√(2πσ²)) × e^(-(x-μ)²/(2σ²))

### Statistics
- **Sample Mean**: x̄ = Σxi/n
- **Sample Variance**: s² = Σ(xi-x̄)²/(n-1)
- **Correlation**: r = Σ((xi-x̄)(yi-ȳ))/√(Σ(xi-x̄)² × Σ(yi-ȳ)²)
- **t-statistic**: t = (x̄-μ₀)/(s/√n)

### Matrix Operations
- **Matrix Multiplication**: (AB)ᵢⱼ = Σₖ AᵢₖBₖⱼ
- **SVD**: A = UΣVᵀ
- **Eigenvalue**: det(A-λI) = 0

---

## Practice Problems for Self-Assessment

### Set 1: Mixed Topics
1. Coin tossed until first head. Expected tosses? (Answer: 2)
2. Distribution of sample mean for n=36 from skewed population? (Answer: Approximately normal)
3. Correlation vs causation example from data science?

### Set 2: Advanced Applications
1. Naive Bayes classifier derivation from Bayes' theorem
2. PCA derivation using eigenvalue decomposition
3. Bootstrap method for confidence intervals

### Set 3: Interview-Style Questions
1. "How would you detect outliers in a dataset?"
2. "Explain the bias-variance tradeoff mathematically."
3. "When would you use median instead of mean?"

---

## Final Tips for the Interview

### Common Pitfalls to Avoid:
- ❌ Confusing correlation with causation
- ❌ Ignoring assumptions of statistical tests
- ❌ Forgetting to mention practical significance
- ❌ Not explaining the intuition behind formulas

### What Interviewers Look For:
- ✅ Clear mathematical reasoning
- ✅ Understanding of assumptions and limitations
- ✅ Practical application knowledge
- ✅ Ability to explain complex concepts simply

### Time Management:
- 📊 **33.33% weightage** for this section
- ⏱️ **30 minutes** allocated
- 🎯 Aim for 2-3 detailed solutions + multiple quick concepts
- 📝 Show your work clearly and systematically