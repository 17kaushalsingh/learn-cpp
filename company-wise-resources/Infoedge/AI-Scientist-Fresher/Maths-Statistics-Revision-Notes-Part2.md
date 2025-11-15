# Maths & Statistics Comprehensive Revision - Part 2

---

## Sampling

### Why Sampling is Critical in Data Science

**Core Insight**: We rarely have access to entire populations. Understanding sampling helps us:
- Make accurate inferences from limited data
- Design effective experiments
- Understand model generalization
- Avoid common biases

### Fundamental Concepts

#### 1. Population vs Sample
**Population**: Complete set of items we're interested in
**Sample**: Subset of population used to make inferences
**Parameter**: True value in population (usually unknown)
**Statistic**: Value calculated from sample (our estimate)

#### 2. Sampling Frame
**Definition**: List or procedure that defines population
**Critical**: Poor sampling frame = biased results regardless of method!

### Sampling Methods with Intuition

#### 1. Simple Random Sample (SRS)
**Intuition**: Every individual has equal chance, every combination equally likely

**Method**:
1. Assign unique numbers to population members
2. Use random number generator to select

**Advantages**:
- Unbiased (if implemented correctly)
- Mathematically tractable
- Easy to understand

**Disadvantages**:
- May not represent subgroups well
- Can be expensive to implement
- Requires complete population list

**Interview Question**: "How would you sample 1000 users from 1M user database randomly?"
**Answer**: Use database's random sampling functions or assign random numbers and select lowest.

#### 2. Stratified Sampling
**Intuition**: Divide population into homogeneous groups, sample from each

**Step-by-Step Process**:
1. **Identify strata**: Mutually exclusive groups (e.g., age groups, regions)
2. **Determine allocation**: How many from each stratum?
3. **Sample within strata**: Use SRS within each group

**Mathematical Allocation**:
```
Proportional: nₕ = (Nₕ/N) × n
Equal: nₕ = n/H (H = number of strata)
Optimal: nₕ = (Nₕ × σₕ) / Σ(Nₕ × σₕ) × n
```

**When to Use**:
- Population has clear subgroups
- Subgroups have different characteristics
- Want to ensure representation of minorities

**Real Interview Question**: "You need to survey customer satisfaction across 3 regions with different populations. How would you sample?"
**Answer**: Stratified sampling by region, proportional to region size.

#### 3. Systematic Sampling
**Intuition**: Select every kth element from ordered list

**Method**:
1. Calculate sampling interval: k = N/n
2. Random start between 1 and k
3. Select every kth element

**Advantages**:
- Simple to implement
- No random number table needed
- Evenly spreads sample

**Risks**: Hidden patterns in ordering can create bias!

#### 4. Cluster Sampling
**Intuition**: Sample groups (clusters) instead of individuals

**Two-Stage Process**:
1. **Stage 1**: Randomly select clusters
2. **Stage 2**: Sample all or some within selected clusters

**When to Use**:
- Natural groupings exist (schools, cities, hospitals)
- Travel costs are high
- Population list unavailable

**Efficiency vs Precision Trade-off**: Cheaper but less precise than SRS

### Sample Size Determination

#### Key Formula for Mean Estimation:
```
n = (Z² × σ²) / E²
```
Where:
- Z = Z-score for confidence level (1.96 for 95%)
- σ = population standard deviation (estimate)
- E = desired margin of error

#### For Proportion:
```
n = (Z² × p × (1-p)) / E²
```

**Interview Problem**: "How large a sample needed to estimate average user rating within ±0.2 with 95% confidence, assuming σ=1.5?"

**Solution**:
n = (1.96² × 1.5²) / 0.2² = (3.8416 × 2.25) / 0.04 = 8.6436 / 0.04 = 216

**Follow-up**: What if we want 99% confidence? (Z = 2.576 → n increases!)

### Sampling Distributions - Bridge to Inference

#### Definition: Distribution of a statistic over all possible samples

**Key Insight**: Sample statistics vary from sample to sample due to random sampling

#### Sampling Distribution of Sample Mean
```
μₓ̄ = μ (unbiased estimator)
σₓ̄ = σ/√n (standard error)
Shape: Approximately normal (CLT)
```

**Interview Application**: "If population mean = 100, σ = 15, n = 25, what's P(sample mean > 105)?"

**Solution**:
- Standard error = 15/√25 = 3
- Z = (105-100)/3 = 5/3 = 1.67
- P(Z > 1.67) = 0.0475

---

## Correlations

### Why Correlation Matters in ML

**Core Insight**: Understanding relationships between variables is fundamental to:
- Feature selection and engineering
- Understanding model behavior
- Detecting multicollinearity
- Exploring data patterns

### Pearson Correlation Coefficient

#### Intuition and Visualization
**Core Idea**: Measures strength and direction of LINEAR relationship

**Visualization of Correlation Values**:
```
r = 1.0    r = 0.7    r = 0.3    r = 0     r = -0.3   r = -0.7   r = -1.0
   ●          ●          ●        ●●●        ●●●        ●●●         ●
   ●          ●         ●●       ●  ●       ●  ●      ●    ●        ●
   ●         ●●        ● ●       ●   ●     ●    ●   ●      ●      ●
   ●        ●  ●      ●   ●      ●   ●     ●    ●   ●      ●      ●
   ●       ●    ●    ●     ●    ●●●●●●●   ●      ●  ●       ●    ●●
   ●      ●      ●  ●       ●           ●         ● ●         ●
   ●     ●        ●●        ●            ●         ●●         ●●
```

#### Mathematical Formula
```
r = Σ((xi - x̄)(yi - ȳ)) / √(Σ(xi - x̄)² × Σ(yi - ȳ)²)
```

**Step-by-Step Calculation**:
1. Calculate means x̄ and ȳ
2. Calculate deviations from means
3. Multiply deviations for each pair
4. Sum the products (numerator)
5. Calculate standard deviations
6. Divide by product of standard deviations

#### Interpretation Guidelines
| r value | Strength | Direction | Practical Meaning |
|---------|----------|-----------|-------------------|
| 0.9 to 1.0 | Very strong | Positive | As X increases, Y increases consistently |
| 0.7 to 0.9 | Strong | Positive | Clear positive relationship |
| 0.5 to 0.7 | Moderate | Positive | Noticeable positive trend |
| 0.3 to 0.5 | Weak | Positive | Slight positive tendency |
| 0.0 to 0.3 | Very weak | Positive | Hardly any relationship |

### Correlation Interview Questions

#### Q1: Complete Calculation (Tests Understanding)
**Question**: Calculate correlation for data: X=[1,2,3,4,5], Y=[2,4,5,4,5]

**Step-by-Step Solution**:
1. **Calculate means**:
   - x̄ = (1+2+3+4+5)/5 = 3
   - ȳ = (2+4+5+4+5)/5 = 4

2. **Calculate deviations and products**:
   ```
   X: 1,2,3,4,5     Deviations: -2,-1,0,1,2
   Y: 2,4,5,4,5     Deviations: -2,0,1,0,1
   Products: 4,0,0,0,2      Sum = 6

   X deviations squared: 4,1,0,1,4    Sum = 10
   Y deviations squared: 4,0,1,0,1    Sum = 6
   ```

3. **Calculate correlation**:
   r = 6 / √(10 × 6) = 6 / √60 = 6 / 7.746 = 0.774

**Interpretation**: Strong positive correlation (0.774)

**Follow-up Questions**:
- What if we square all X values? (Correlation becomes lower)
- How would outliers affect this correlation?
- What's the coefficient of determination? (r² = 0.774² = 0.599)

#### Q2: Correlation vs Causation (Critical Thinking)
**Question**: "Ice cream sales and drowning deaths are highly correlated (r=0.85). Does eating ice cream cause drowning?"

**Step-by-Step Analysis**:
1. **Identify correlation**: Strong positive correlation exists
2. **Consider confounding variable**: Temperature/season affects both
3. **Mechanism check**: No biological mechanism for ice cream → drowning
4. **Temporal relationship**: Both increase in summer months
5. **Conclusion**: Correlation ≠ causation, likely confounding by season

**Interview Insight**: Tests critical thinking and scientific reasoning!

#### Q3: Non-linear Relationships (Tests Limitation Understanding)
**Question**: "Can perfect correlation exist for non-linear relationships?"

**Answer**: No! Pearson correlation only measures LINEAR relationships.

**Example**: Y = X² for X ∈ [-2,2]
- Perfect functional relationship
- But correlation = 0 (symmetric around 0)
- Need other measures for non-linear relationships

**Follow-up**: What correlation measure would capture this? (Use Spearman correlation)

### Advanced Correlation Concepts

#### 1. Spearman Rank Correlation
**When to Use**: Non-linear monotonic relationships, ordinal data

**Method**: Correlate ranks instead of actual values

**Advantages**:
- Robust to outliers
- Captures monotonic (always increasing/decreasing) relationships
- Works with ordinal data

#### 2. Kendall's Tau
**When to Use**: Small datasets, ordinal data

**Intuition**: Measures concordance vs discordance in pairs

#### 3. Partial Correlation
**Definition**: Correlation between X and Y controlling for Z

**Formula**: rₓᵧ.𝓏 = (rₓᵧ - rₓ𝓻rᵧ𝓻) / √((1-rₓ𝓻²)(1-rᵧ𝓻²))

**Application**: Is correlation between height and weight real or just due to age?

### Correlation in Machine Learning

#### Feature Selection
**High correlation between features**:
- Risk of multicollinearity
- Redundant information
- Consider dimensionality reduction

**High correlation with target**:
- Good predictive features
- Consider for feature engineering

#### Interview Question: Feature Engineering
**Question**: "You have 20 features, some highly correlated. How would you handle this?"

**Approach**:
1. **Calculate correlation matrix**
2. **Identify highly correlated pairs** (|r| > 0.8)
3. **Options**:
   - Remove one of each correlated pair
   - Use PCA for dimensionality reduction
   - Combine correlated features (averaging, domain knowledge)
4. **Consider domain knowledge**: Some correlations are meaningful

### Common Correlation Mistakes in Interviews

#### ❌ Mistakes to Avoid:
1. **Assuming linearity**: Not mentioning limitations of Pearson
2. **Confusing correlation with causation**: Classic interview trap
3. **Ignoring outliers**: Not mentioning robustness issues
4. **Wrong correlation type**: Using Pearson for ordinal data
5. **Multiple testing**: Not mentioning Bonferroni correction

#### ✅ Pro Strategies:
1. **Always mention assumptions**: Linearity, normality, no outliers
2. **Discuss visualization**: "I'd first plot the data"
3. **Consider robustness**: "I'd also check Spearman correlation"
4. **Domain context**: Correlation meaning depends on field
5. **Multiple comparisons**: Adjust for testing many correlations

### Quick Correlation Reference

| Correlation Type | Data Type | Relationship | When to Use |
|------------------|-----------|-------------|-------------|
| Pearson | Continuous | Linear | Most common, parametric |
| Spearman | Any | Monotonic | Non-linear, outliers |
| Kendall | Any | Monotonic | Small datasets, ordinal |
| Point-biserial | Binary + Continuous | Linear | One binary variable |
| Phi | Two binary | Linear | Two binary variables |

---

## Hypothesis Testing

### The Foundation of Statistical Inference

**Core Insight**: Hypothesis testing provides a framework for making decisions under uncertainty. It's the backbone of A/B testing, clinical trials, and most scientific research.

### Fundamental Framework

#### The Logic of Hypothesis Testing
```
Assume H₀ is true → Calculate probability of observed data →
If probability is small → Reject H₀ in favor of H₁
```

**Key Components**:
- **Null Hypothesis (H₀)**: No effect, no difference, status quo
- **Alternative Hypothesis (H₁)**: Effect exists, difference present
- **Test Statistic**: Standardized measure of evidence
- **Critical Region**: Values that lead to H₀ rejection
- **Significance Level (α)**: Threshold for decision-making

#### Type I and Type II Errors

```
                      Reality
                H₀ True       H₀ False
Decision:
Reject H₀        Type I        Correct
Don't Reject     Correct       Type II
```

**Type I Error (α)**: False positive - rejecting true H₀
**Type II Error (β)**: False negative - failing to reject false H₀
**Power (1-β)**: Probability of correctly rejecting false H₀

### Step-by-Step Hypothesis Testing Process

#### Step 1: Formulate Hypotheses
**H₀**: μ = μ₀ (no difference from hypothesized value)
**H₁**: μ ≠ μ₀ (two-tailed) OR μ > μ₀ (right-tailed) OR μ < μ₀ (left-tailed)

#### Step 2: Choose Significance Level
**Common choices**: α = 0.05 (95% confidence), α = 0.01 (99% confidence)

**Trade-off**: Lower α = more stringent but higher Type II error risk

#### Step 3: Select Appropriate Test
**One-sample tests**: Compare sample to population
**Two-sample tests**: Compare two samples
**Paired tests**: Same subjects, different conditions

#### Step 4: Calculate Test Statistic
**Standard formula**: (statistic - hypothesized value) / standard error

#### Step 5: Make Decision
**Critical value approach**: Compare to threshold
**p-value approach**: P(observed or more extreme | H₀ true)

### One-Sample Tests

#### One-Sample Z-Test
**When to Use**:
- Population σ known
- Sample size n ≥ 30 (CLT applies)
- Population approximately normal

**Test Statistic**: z = (x̄ - μ₀) / (σ/√n)

#### One-Sample T-Test
**When to Use**:
- Population σ unknown (most common case)
- Sample size n < 30
- Population approximately normal

**Test Statistic**: t = (x̄ - μ₀) / (s/√n)

**Degrees of Freedom**: df = n - 1

#### One-Sample T-Test Interview Question

**Question**: "A company claims average battery life = 10 hours. Sample of 25 batteries shows x̄ = 9.5 hours, s = 1.2 hours. Test at α = 0.05."

**Step-by-Step Solution**:

1. **Formulate hypotheses**:
   - H₀: μ = 10 hours
   - H₁: μ ≠ 10 hours (two-tailed)

2. **Choose test**: One-sample t-test (σ unknown, n=25)

3. **Calculate test statistic**:
   t = (9.5 - 10) / (1.2/√25) = (-0.5) / (1.2/5) = (-0.5) / 0.24 = -2.083

4. **Critical values**: t(24, 0.025) = ±2.064

5. **Decision**: |t| = 2.083 > 2.064 → Reject H₀

6. **Conclusion**: Evidence suggests battery life differs from claim

**Follow-up Questions**:
- What's the p-value? (≈ 0.048)
- How would confidence interval approach work?
- What assumptions are we making?

### Two-Sample Tests

#### Independent Two-Sample T-Test
**When to Use**: Compare means of two independent groups

**Equal Variances Assumed**:
```
t = (x̄₁ - x̄₂) / √(sp² × (1/n₁ + 1/n₂))
where sp² = ((n₁-1)s₁² + (n₂-1)s₂²)/(n₁+n₂-2)
```

**Unequal Variances (Welch's t-test)**:
```
t = (x̄₁ - x̄₂) / √(s₁²/n₁ + s₂²/n₂)
```

#### Two-Sample T-Test Interview Question

**Question**: "A/B test: Version A (n₁=100, x̄₁=52.3, s₁=8.1), Version B (n₂=100, x̄₂=54.7, s₂=7.9). Test if B significantly better."

**Step-by-Step Solution**:

1. **Hypotheses**:
   - H₀: μ₁ = μ₂
   - H₁: μ₁ < μ₂ (right-tailed, B better)

2. **Test choice**: Two-sample t-test, assume equal variances

3. **Pooled variance**:
   sp² = ((99×8.1² + 99×7.9²)/198) = ((99×65.61 + 99×62.41)/198)
   sp² = (6495.39 + 6178.59)/198 = 62.36

4. **Standard error**: √(62.36 × (1/100 + 1/100)) = √(62.36 × 0.02) = √1.247 = 1.117

5. **Test statistic**: t = (52.3 - 54.7)/1.117 = -2.4/1.117 = -2.149

6. **Critical value**: t(198, 0.05) ≈ -1.652

7. **Decision**: t = -2.149 < -1.652 → Reject H₀

8. **Conclusion**: Version B significantly better than A

**Interview Insight**: This is exactly how A/B testing works in practice!

### Paired T-Test

#### When to Use Paired Tests
- Same subjects measured twice (before/after)
- Matched pairs (twins, matched groups)
- Repeated measures designs

#### Advantages Over Independent Tests
- Controls for individual differences
- Increases statistical power
- Reduces required sample size

#### Paired T-Test Interview Question

**Question**: "Weight loss program: Before weights [180,170,165], After weights [175,168,160]. Test effectiveness."

**Step-by-Step Solution**:

1. **Calculate differences**: [180-175, 170-168, 165-160] = [5, 2, 5]

2. **Calculate statistics for differences**:
   - d̄ = (5 + 2 + 5)/3 = 4
   - Differences from mean: [1, -2, 1]
   - Squared differences: [1, 4, 1]
   - s_d = √((1+4+1)/(3-1)) = √(6/2) = √3 = 1.732

3. **Test statistic**:
   t = d̄ / (s_d/√n) = 4 / (1.732/√3) = 4 / 1 = 4

4. **Critical value**: t(2, 0.05) = 2.92

5. **Decision**: t = 4 > 2.92 → Reject H₀

6. **Conclusion**: Significant weight loss

**Follow-up**: Calculate 95% CI for mean weight loss: d̄ ± t(2,0.025) × (s_d/√3)

### Chi-Square Tests

#### Goodness of Fit Test
**When to Use**: Compare observed frequencies to expected frequencies

**Test Statistic**: χ² = Σ((O - E)²/E)

**Degrees of Freedom**: df = categories - 1 - parameters estimated

#### Chi-Square Goodness of Fit Interview Question

**Question**: "Die rolled 60 times, results: [1:8, 2:12, 3:9, 4:11, 5:10, 6:10]. Is die fair?"

**Step-by-Step Solution**:

1. **Hypotheses**:
   - H₀: Die is fair (each face probability = 1/6)
   - H₁: Die is not fair

2. **Expected frequencies**: 60 × 1/6 = 10 for each face

3. **Calculate chi-square**:
   χ² = ((8-10)²/10) + ((12-10)²/10) + ((9-10)²/10) + ((11-10)²/10) + ((10-10)²/10) + ((10-10)²/10)
   χ² = (4/10) + (4/10) + (1/10) + (1/10) + 0 + 0 = 1.0

4. **Critical value**: χ²(5, 0.05) = 11.07

5. **Decision**: 1.0 < 11.07 → Fail to reject H₀

6. **Conclusion**: No evidence die is unfair

### P-Value Approach

#### Definition and Interpretation
**P-value**: Probability of observing test statistic as extreme or more extreme, assuming H₀ is true

**Interpretation Guidelines**:
- p < 0.01: Very strong evidence against H₀
- 0.01 ≤ p < 0.05: Strong evidence against H₀
- 0.05 ≤ p < 0.10: Moderate evidence against H₀
- p ≥ 0.10: Little or no evidence against H₀

#### Common Misinterpretations
❌ "P-value is probability H₀ is true"
❌ "P-value is probability results occurred by chance"
✅ "P-value is probability of results this extreme, assuming H₀ is true"

### Multiple Testing Problem

#### The Issue
**Problem**: When testing multiple hypotheses, probability of at least one Type I error increases dramatically

**Example**: 20 independent tests at α = 0.05
P(at least one Type I error) = 1 - (1-0.05)²⁰ = 0.64 (64% chance!)

#### Bonferroni Correction
**Method**: α_corrected = α / number_of_tests

**Trade-off**: Controls Type I error but increases Type II error

### Hypothesis Testing in Machine Learning

#### Feature Selection
**Goal**: Test if features are significantly associated with target
**Methods**: t-tests, chi-square tests, ANOVA

#### Model Comparison
**Goal**: Test if one model significantly outperforms another
**Methods**: Paired t-test on cross-validation scores

#### Statistical Significance vs Practical Significance
**Key Insight**: With large samples, even tiny differences become statistically significant
**Solution**: Always consider effect size and practical importance

### Common Hypothesis Testing Interview Mistakes

#### ❌ Critical Mistakes:
1. **Wrong test selection**: Using z-test when t-test needed
2. **Confusing one-tailed and two-tailed**: Not justifying direction
3. **Ignoring assumptions**: Not checking normality, equal variances
4. **Multiple testing**: Not correcting for multiple comparisons
5. **Misinterpreting p-values**: Common statistical fallacy

#### ✅ Pro Interview Strategies:
1. **Always state hypotheses clearly**: Define H₀ and H₁
2. **Explain test choice**: Why this test, what assumptions
3. **Show calculations step-by-step**: Demonstrate understanding
4. **Discuss assumptions and limitations**: Critical thinking
5. **Connect to practical significance**: Beyond statistical significance
6. **Consider alternative approaches**: Confidence intervals, effect sizes

### Quick Reference for Test Selection

| Situation | Test | Assumptions | When to Use |
|-----------|------|-------------|-------------|
| One sample vs population | One-sample t-test | Normality, σ unknown | n < 30, σ unknown |
| Two independent samples | Two-sample t-test | Normality, equal variances | Compare two groups |
| Same subjects twice | Paired t-test | Normality of differences | Before/after studies |
| Categorical data | Chi-square test | Expected frequencies ≥ 5 | Goodness of fit, independence |
| More than two groups | ANOVA | Normality, equal variances | Compare multiple means |