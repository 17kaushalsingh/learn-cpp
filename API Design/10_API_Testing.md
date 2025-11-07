# Chapter 10: API Testing

## Introduction to API Testing

API testing is the process of verifying that APIs function correctly, reliably, and perform well under various conditions. Unlike UI testing, API testing focuses on the business logic layer, testing functionality, performance, security, and reliability.

### Why API Testing Matters

- **Early Bug Detection**: Find issues before they reach the UI layer
- **Performance Validation**: Ensure APIs handle expected loads
- **Contract Verification**: Validate API specifications are met
- **Regression Prevention**: Catch breaking changes early
- **Integration Confidence**: Ensure systems work together properly

## Load Testing

### What is Load Testing?

Load testing evaluates API performance under expected user loads to identify bottlenecks and ensure the system can handle concurrent requests.

### Key Metrics to Monitor

```javascript
// Load Testing Metrics
const loadTestMetrics = {
  responseTime: {
    average: 'Mean response time across all requests',
    p50: '50th percentile response time',
    p95: '95th percentile response time (critical for UX)',
    p99: '99th percentile response time',
    max: 'Maximum response time'
  },

  throughput: {
    requestsPerSecond: 'Total requests processed per second',
    dataTransferred: 'Amount of data transferred per second',
    errorRate: 'Percentage of failed requests'
  },

  resourceUtilization: {
    cpuUsage: 'Server CPU consumption',
    memoryUsage: 'RAM consumption',
    diskIO: 'Disk read/write operations',
    networkIO: 'Network bandwidth usage'
  },

  reliability: {
    availability: 'Percentage of time API is responsive',
    mttr: 'Mean time to recovery from failures',
    errorDistribution: 'Types and frequency of errors'
  }
};
```

### Load Testing Tools

#### 1. Apache Bench (ApacheBench)

```bash
# Basic load test with Apache Bench
ab -n 1000 -c 100 https://api.example.com/users

# Detailed output example:
# This is ApacheBench, Version 2.3 <$Revision: 1879490 $>
# Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
# Licensed to The Apache Software Foundation, http://www.apache.org/

# Benchmarking api.example.com (be patient)
# Completed 100 requests
# Completed 200 requests
# Completed 300 requests
# ...

# Server Software:        nginx/1.18.0
# Server Hostname:        api.example.com
# Server Port:            443
# SSL/TLS Protocol:       TLSv1.2,ECDHE-RSA-AES256-GCM-SHA384,2048-bit

# Document Path:          /users
# Document Length:        1256 bytes

# Concurrency Level:      100
# Time taken for tests:   2.345 seconds
# Complete requests:      1000
# Failed requests:        0
# Total transferred:      1456000 bytes
# HTML transferred:       1256000 bytes
# Requests per second:    426.44 [#/sec] (mean)
# Time per request:       234.567 [ms] (mean)
# Time per request:       2.346 [ms] (mean, across all concurrent requests)
# Transfer rate:          606.34 [Kbytes/sec] received
```

#### 2. Artillery.js

```yaml
# artillery.yml
config:
  target: 'https://api.example.com'
  phases:
    - duration: 60
      arrivalRate: 10
      name: "Warm up"
    - duration: 120
      arrivalRate: 50
      name: "Ramp up load"
    - duration: 300
      arrivalRate: 100
      name: "Sustained load"
    - duration: 60
      arrivalRate: 200
      name: "Peak load"

scenarios:
  - name: "Get users"
    requests:
      - get:
          url: "/users"
          headers:
            Authorization: "Bearer {{ token }}"

  - name: "Create user"
    requests:
      - post:
          url: "/users"
          headers:
            Content-Type: "application/json"
            Authorization: "Bearer {{ token }}"
          json:
            name: "Test User"
            email: "test@example.com"
```

```javascript
// artillery run artillery.yml
// Output example:
// Metrics for scenario:
//   virtual users: 100
//   created: 6000
//   completed: 5998
//
// HTTP responses:
//   200 OK: 5998
//
// Counts:
#    p50: 145ms
#    p95: 280ms
#    p99: 450ms
#    max: 1200ms
#
# Throughput:
#   request.rate: 99.97 req/sec
#   latency.avg: 148ms
```

#### 3. K6 (Modern Load Testing)

```javascript
// load-test.js
import http from 'k6/http';
import { check, sleep } from 'k6';
import { Rate } from 'k6/metrics';

export let errorRate = new Rate('errors');

export let options = {
  stages: [
    { duration: '2m', target: 100 }, // ramp up to 100 users
    { duration: '5m', target: 100 }, // stay at 100 users
    { duration: '2m', target: 200 }, // ramp up to 200 users
    { duration: '5m', target: 200 }, // stay at 200 users
    { duration: '2m', target: 0 },   // ramp down
  ],
  thresholds: {
    http_req_duration: ['p(95)<500'], // 95% of requests under 500ms
    http_req_failed: ['rate<0.1'],    // error rate under 10%
    errors: ['rate<0.1'],             // custom error metric under 10%
  },
};

const BASE_URL = 'https://api.example.com';

export function setup() {
  // Setup: Get auth token
  const loginResponse = http.post(`${BASE_URL}/auth/login`, {
    email: 'test@example.com',
    password: 'testpassword'
  });

  return {
    token: loginResponse.json('token')
  };
}

export default function(data) {
  const params = {
    headers: {
      'Authorization': `Bearer ${data.token}`,
      'Content-Type': 'application/json',
    },
  };

  // Test GET /users
  let usersResponse = http.get(`${BASE_URL}/users`, params);
  let success = check(usersResponse, {
    'users status is 200': (r) => r.status === 200,
    'users response time < 500ms': (r) => r.timings.duration < 500,
    'users returned data': (r) => r.json().length > 0,
  });

  errorRate.add(!success);

  // Test POST /users
  let createUserResponse = http.post(`${BASE_URL}/users`,
    JSON.stringify({
      name: `User ${Date.now()}`,
      email: `user${Date.now()}@example.com`
    }),
    params
  );

  success = check(createUserResponse, {
    'create status is 201': (r) => r.status === 201,
    'create response time < 1000ms': (r) => r.timings.duration < 1000,
    'user created successfully': (r) => r.json('id') !== undefined,
  });

  errorRate.add(!success);

  sleep(1);
}

export function teardown(data) {
  // Cleanup: Delete test users if needed
  console.log('Load test completed');
}
```

```bash
# Run K6 load test
k6 run load-test.js

# Output example:
#          /\      |‾‾| /‾‾/   /‾‾/
#     /\  /  \     |  |/  /   /  /
#    /  \/    \    |     (   /   ‾‾\
#   /          \   |  |\  \ |  (‾)  |
#  / __________ \  |__| \__\ \_____/
#
# execution: local
#    script: load-test.js
#    output: -
#
#    scenarios: (100.00%) 1 scenario, 200 max VUs, 10m30s max duration
#              default: Up to 200 looping VUs for 16m0s over 5 stages
#
# running (16m00s), 000/200 VUs, 1000 complete and 0 interrupted iterations
# default ✓ [======================================] 200 VUs  16m00s
#
#      ✓ users status is 200
#      ✓ users response time < 500ms
#      ✓ users returned data
#      ✓ create status is 201
#      ✓ create response time < 1000ms
#      ✓ user created successfully
#
#      checks.....................: 100.00% ✓ 6000      ✗ 0
#      data_received..................: 2.1 MB 2.2 kB/s
#      data_sent......................: 2.8 MB 2.9 kB/s
#      http_req_blocked...............: avg=2.34ms  min=0s    med=1ms   max=145ms p(90)=4ms   p(95)=6ms
#      http_req_connecting............: avg=15.67ms min=0s    med=12ms  max=200ms p(90)=25ms  p(95)=35ms
#      iterations.....................: 1000    1.041239/s
```

## Functional Testing

### What is Functional Testing?

Functional testing verifies that API functions work according to specifications, focusing on business logic, input validation, and output accuracy.

### Functional Testing Patterns

#### 1. Request-Response Validation

```javascript
// Functional Test Suite for User API
describe('User API Functional Tests', () => {
  let authToken;
  let testUserId;

  beforeAll(async () => {
    // Setup: Get authentication token
    const loginResponse = await request(app)
      .post('/api/auth/login')
      .send({
        email: 'admin@example.com',
        password: 'adminpassword'
      });

    authToken = loginResponse.body.token;
    expect(authToken).toBeDefined();
  });

  describe('POST /api/users - Create User', () => {
    test('should create user with valid data', async () => {
      const userData = {
        name: 'John Doe',
        email: 'john@example.com',
        password: 'securePassword123',
        role: 'user'
      };

      const response = await request(app)
        .post('/api/users')
        .set('Authorization', `Bearer ${authToken}`)
        .send(userData);

      expect(response.status).toBe(201);
      expect(response.body).toMatchObject({
        name: userData.name,
        email: userData.email,
        role: userData.role
      });
      expect(response.body.id).toBeDefined();
      expect(response.body.password).toBeUndefined(); // Password should not be returned

      testUserId = response.body.id;
    });

    test('should reject user creation with invalid email', async () => {
      const userData = {
        name: 'Jane Doe',
        email: 'invalid-email',
        password: 'password123'
      };

      const response = await request(app)
        .post('/api/users')
        .set('Authorization', `Bearer ${authToken}`)
        .send(userData);

      expect(response.status).toBe(400);
      expect(response.body.errors).toContainEqual({
        field: 'email',
        message: 'Invalid email format'
      });
    });

    test('should reject user creation with weak password', async () => {
      const userData = {
        name: 'Weak User',
        email: 'weak@example.com',
        password: '123'
      };

      const response = await request(app)
        .post('/api/users')
        .set('Authorization', `Bearer ${authToken}`)
        .send(userData);

      expect(response.status).toBe(400);
      expect(response.body.errors).toContainEqual({
        field: 'password',
        message: 'Password must be at least 8 characters long'
      });
    });

    test('should require authentication', async () => {
      const userData = {
        name: 'Unauthorized User',
        email: 'unauth@example.com',
        password: 'password123'
      };

      const response = await request(app)
        .post('/api/users')
        .send(userData);

      expect(response.status).toBe(401);
      expect(response.body.message).toBe('Authentication required');
    });
  });

  describe('GET /api/users/:id - Get User', () => {
    test('should return user by ID', async () => {
      const response = await request(app)
        .get(`/api/users/${testUserId}`)
        .set('Authorization', `Bearer ${authToken}`);

      expect(response.status).toBe(200);
      expect(response.body.id).toBe(testUserId);
      expect(response.body.name).toBeDefined();
      expect(response.body.email).toBeDefined();
      expect(response.body.password).toBeUndefined();
    });

    test('should return 404 for non-existent user', async () => {
      const fakeId = '507f1f77bcf86cd799439011'; // Valid ObjectId format

      const response = await request(app)
        .get(`/api/users/${fakeId}`)
        .set('Authorization', `Bearer ${authToken}`);

      expect(response.status).toBe(404);
      expect(response.body.message).toBe('User not found');
    });

    test('should validate ID format', async () => {
      const invalidId = 'invalid-id-format';

      const response = await request(app)
        .get(`/api/users/${invalidId}`)
        .set('Authorization', `Bearer ${authToken}`);

      expect(response.status).toBe(400);
      expect(response.body.message).toBe('Invalid user ID format');
    });
  });

  describe('PUT /api/users/:id - Update User', () => {
    test('should update user with valid data', async () => {
      const updateData = {
        name: 'Updated Name',
        role: 'moderator'
      };

      const response = await request(app)
        .put(`/api/users/${testUserId}`)
        .set('Authorization', `Bearer ${authToken}`)
        .send(updateData);

      expect(response.status).toBe(200);
      expect(response.body).toMatchObject(updateData);
      expect(response.body.id).toBe(testUserId);
    });

    test('should reject update with invalid role', async () => {
      const updateData = {
        role: 'invalid-role'
      };

      const response = await request(app)
        .put(`/api/users/${testUserId}`)
        .set('Authorization', `Bearer ${authToken}`)
        .send(updateData);

      expect(response.status).toBe(400);
      expect(response.body.errors).toContainEqual({
        field: 'role',
        message: 'Role must be one of: user, moderator, admin'
      });
    });
  });

  describe('DELETE /api/users/:id - Delete User', () => {
    test('should delete user successfully', async () => {
      const response = await request(app)
        .delete(`/api/users/${testUserId}`)
        .set('Authorization', `Bearer ${authToken}`);

      expect(response.status).toBe(200);
      expect(response.body.message).toBe('User deleted successfully');
    });

    test('should return 404 when trying to delete already deleted user', async () => {
      const response = await request(app)
        .delete(`/api/users/${testUserId}`)
        .set('Authorization', `Bearer ${authToken}`);

      expect(response.status).toBe(404);
      expect(response.body.message).toBe('User not found');
    });
  });
});
```

#### 2. Data Validation Testing

```javascript
// Comprehensive Input Validation Tests
describe('Input Validation Tests', () => {
  const testCases = [
    {
      field: 'email',
      validInputs: [
        'user@example.com',
        'user.name+tag@example.co.uk',
        'user123@test-domain.com'
      ],
      invalidInputs: [
        'invalid-email',
        '@example.com',
        'user@',
        'user..name@example.com',
        '',
        null,
        undefined
      ]
    },
    {
      field: 'name',
      validInputs: [
        'John Doe',
        'Álvaro Martínez',
        '张三',
        'John-Doe O\'Connor'
      ],
      invalidInputs: [
        '',
        'a', // too short
        'a'.repeat(101), // too long
        '123', // numbers only
        '!@#$%', // special chars only
        null,
        undefined
      ]
    },
    {
      field: 'age',
      validInputs: [18, 25, 65, 100],
      invalidInputs: [-1, 0, 17, 101, 150, 'invalid', null, undefined]
    }
  ];

  testCases.forEach(({ field, validInputs, invalidInputs }) => {
    describe(`${field} validation`, () => {
      test.each(validInputs)('should accept valid %s: %p', (input) => {
        const userData = {
          name: 'Test User',
          email: 'test@example.com',
          [field]: input
        };

        const response = request(app)
          .post('/api/users')
          .set('Authorization', `Bearer ${authToken}`)
          .send(userData);

        expect(response.status).not.toBe(400);
      });

      test.each(invalidInputs)('should reject invalid %s: %p', (input) => {
        const userData = {
          name: 'Test User',
          email: 'test@example.com',
          [field]: input
        };

        const response = request(app)
          .post('/api/users')
          .set('Authorization', `Bearer ${authToken}`)
          .send(userData);

        expect(response.status).toBe(400);
      });
    });
  });
});
```

## Integration Testing

### What is Integration Testing?

Integration testing verifies that different API endpoints work together correctly, testing the interaction between various components and ensuring the system behaves as expected as a whole.

### Integration Testing Strategies

#### 1. Database Integration Testing

```javascript
// Database Integration Tests
describe('Database Integration Tests', () => {
  let dbConnection;
  let testUser;

  beforeAll(async () => {
    // Setup test database
    dbConnection = await mongoose.connect(process.env.TEST_DB_URI);

    // Clear test collections
    await User.deleteMany({});
    await Order.deleteMany({});
  });

  afterAll(async () => {
    // Cleanup
    await User.deleteMany({});
    await Order.deleteMany({});
    await dbConnection.close();
  });

  describe('User-Order Integration', () => {
    test('should create user and associated orders', async () => {
      // Step 1: Create user
      const userResponse = await request(app)
        .post('/api/users')
        .send({
          name: 'Test Customer',
          email: 'customer@example.com',
          password: 'password123'
        });

      expect(userResponse.status).toBe(201);
      testUser = userResponse.body;

      // Step 2: Create orders for the user
      const order1Response = await request(app)
        .post('/api/orders')
        .set('Authorization', `Bearer ${authToken}`)
        .send({
          userId: testUser.id,
          items: [
            { productId: 'prod1', quantity: 2, price: 29.99 },
            { productId: 'prod2', quantity: 1, price: 49.99 }
          ]
        });

      const order2Response = await request(app)
        .post('/api/orders')
        .set('Authorization', `Bearer ${authToken}`)
        .send({
          userId: testUser.id,
          items: [
            { productId: 'prod3', quantity: 3, price: 19.99 }
          ]
        });

      expect(order1Response.status).toBe(201);
      expect(order2Response.status).toBe(201);

      // Step 3: Verify orders are associated with user
      const userOrdersResponse = await request(app)
        .get(`/api/users/${testUser.id}/orders`)
        .set('Authorization', `Bearer ${authToken}`);

      expect(userOrdersResponse.status).toBe(200);
      expect(userOrdersResponse.body).toHaveLength(2);

      // Step 4: Verify database relationships
      const userInDb = await User.findById(testUser.id).populate('orders');
      expect(userInDb.orders).toHaveLength(2);
    });

    test('should handle cascading deletes correctly', async () => {
      // Create user with orders
      const user = await User.create({
        name: 'Cascade Test User',
        email: 'cascade@example.com'
      });

      await Order.create([
        { userId: user._id, items: [{ productId: 'prod1', quantity: 1, price: 10 }] },
        { userId: user._id, items: [{ productId: 'prod2', quantity: 2, price: 20 }] }
      ]);

      // Delete user
      const deleteResponse = await request(app)
        .delete(`/api/users/${user._id}`)
        .set('Authorization', `Bearer ${authToken}`);

      expect(deleteResponse.status).toBe(200);

      // Verify orders are also deleted or handled correctly
      const orphanedOrders = await Order.find({ userId: user._id });
      expect(orphanedOrders).toHaveLength(0);
    });
  });
});
```

#### 2. Third-Party Service Integration

```javascript
// Third-Party Service Integration Tests
describe('Payment Service Integration', () => {
  beforeAll(() => {
    // Mock external payment service for testing
    jest.mock('stripe', () => {
      return jest.fn().mockImplementation(() => ({
        charges: {
          create: jest.fn().mockResolvedValue({
            id: 'ch_test123',
            status: 'succeeded',
            amount: 2999
          })
        },
        refunds: {
          create: jest.fn().mockResolvedValue({
            id: 're_test123',
            status: 'succeeded'
          })
        }
      }));
    });
  });

  test('should process payment through external service', async () => {
    const paymentData = {
      amount: 2999, // $29.99 in cents
      currency: 'usd',
      source: 'tok_test123',
      orderId: 'order123'
    };

    const response = await request(app)
      .post('/api/payments/charge')
      .set('Authorization', `Bearer ${authToken}`)
      .send(paymentData);

    expect(response.status).toBe(200);
    expect(response.body.status).toBe('succeeded');
    expect(response.body.chargeId).toBe('ch_test123');
  });

  test('should handle payment failures gracefully', async () => {
    // Override mock to simulate failure
    const stripe = require('stripe')();
    stripe.charges.create.mockRejectedValueOnce(new Error('Card declined'));

    const paymentData = {
      amount: 2999,
      currency: 'usd',
      source: 'tok_declined',
      orderId: 'order123'
    };

    const response = await request(app)
      .post('/api/payments/charge')
      .set('Authorization', `Bearer ${authToken}`)
      .send(paymentData);

    expect(response.status).toBe(400);
    expect(response.body.error).toBe('Payment failed');
  });
});
```

## Unit Testing

### What is Unit Testing for APIs?

Unit testing focuses on testing individual functions, methods, or components in isolation. For APIs, this typically means testing controllers, services, and utilities independently.

### Unit Testing Patterns

#### 1. Controller Unit Tests

```javascript
// User Controller Unit Tests
describe('UserController Unit Tests', () => {
  let mockRequest;
  let mockResponse;
  let userController;
  let mockUserService;

  beforeEach(() => {
    mockRequest = {
      params: {},
      body: {},
      user: { id: 'user123' }
    };

    mockResponse = {
      status: jest.fn().mockReturnThis(),
      json: jest.fn().mockReturnThis(),
      send: jest.fn().mockReturnThis()
    };

    mockUserService = {
      createUser: jest.fn(),
      getUserById: jest.fn(),
      updateUser: jest.fn(),
      deleteUser: jest.fn()
    };

    userController = new UserController(mockUserService);
  });

  describe('createUser', () => {
    test('should create user successfully', async () => {
      const userData = {
        name: 'Test User',
        email: 'test@example.com',
        password: 'password123'
      };

      const expectedUser = {
        id: 'user123',
        name: userData.name,
        email: userData.email,
        role: 'user'
      };

      mockRequest.body = userData;
      mockUserService.createUser.mockResolvedValue(expectedUser);

      await userController.createUser(mockRequest, mockResponse);

      expect(mockUserService.createUser).toHaveBeenCalledWith(userData);
      expect(mockResponse.status).toHaveBeenCalledWith(201);
      expect(mockResponse.json).toHaveBeenCalledWith(expectedUser);
    });

    test('should handle validation errors', async () => {
      const invalidData = {
        name: 'Test',
        email: 'invalid-email'
        // missing password
      };

      mockRequest.body = invalidData;
      mockUserService.createUser.mockRejectedValue(new Error('Invalid email format'));

      await userController.createUser(mockRequest, mockResponse);

      expect(mockResponse.status).toHaveBeenCalledWith(400);
      expect(mockResponse.json).toHaveBeenCalledWith({
        error: 'Invalid email format'
      });
    });

    test('should handle duplicate email error', async () => {
      const userData = {
        name: 'Duplicate User',
        email: 'existing@example.com',
        password: 'password123'
      };

      mockRequest.body = userData;
      mockUserService.createUser.mockRejectedValue(new Error('Email already exists'));

      await userController.createUser(mockRequest, mockResponse);

      expect(mockResponse.status).toHaveBeenCalledWith(409);
      expect(mockResponse.json).toHaveBeenCalledWith({
        error: 'Email already exists'
      });
    });
  });

  describe('getUserById', () => {
    test('should return user when found', async () => {
      const userId = 'user123';
      const expectedUser = {
        id: userId,
        name: 'Test User',
        email: 'test@example.com'
      };

      mockRequest.params.id = userId;
      mockUserService.getUserById.mockResolvedValue(expectedUser);

      await userController.getUserById(mockRequest, mockResponse);

      expect(mockUserService.getUserById).toHaveBeenCalledWith(userId);
      expect(mockResponse.status).toHaveBeenCalledWith(200);
      expect(mockResponse.json).toHaveBeenCalledWith(expectedUser);
    });

    test('should return 404 when user not found', async () => {
      const userId = 'nonexistent';
      mockRequest.params.id = userId;
      mockUserService.getUserById.mockResolvedValue(null);

      await userController.getUserById(mockRequest, mockResponse);

      expect(mockResponse.status).toHaveBeenCalledWith(404);
      expect(mockResponse.json).toHaveBeenCalledWith({
        error: 'User not found'
      });
    });
  });
});
```

#### 2. Service Layer Unit Tests

```javascript
// User Service Unit Tests
describe('UserService Unit Tests', () => {
  let userService;
  let mockUserModel;
  let mockEmailService;

  beforeEach(() => {
    mockUserModel = {
      create: jest.fn(),
      findById: jest.fn(),
      findOneAndUpdate: jest.fn(),
      findByIdAndDelete: jest.fn(),
      findOne: jest.fn()
    };

    mockEmailService = {
      sendWelcomeEmail: jest.fn(),
      sendPasswordResetEmail: jest.fn()
    };

    userService = new UserService(mockUserModel, mockEmailService);
  });

  describe('createUser', () => {
    test('should create user with hashed password', async () => {
      const userData = {
        name: 'Test User',
        email: 'test@example.com',
        password: 'plainpassword'
      };

      const expectedCreatedUser = {
        _id: 'user123',
        name: userData.name,
        email: userData.email,
        password: 'hashedpassword',
        role: 'user'
      };

      mockUserModel.create.mockResolvedValue(expectedCreatedUser);
      mockEmailService.sendWelcomeEmail.mockResolvedValue(true);

      const result = await userService.createUser(userData);

      expect(mockUserModel.create).toHaveBeenCalledWith(
        expect.objectContaining({
          name: userData.name,
          email: userData.email,
          password: expect.any(String) // Should be hashed
        })
      );

      expect(result.password).toBeUndefined(); // Password should not be returned
      expect(mockEmailService.sendWelcomeEmail).toHaveBeenCalledWith(userData.email);
    });

    test('should throw error if email already exists', async () => {
      const userData = {
        name: 'Test User',
        email: 'existing@example.com',
        password: 'password123'
      };

      mockUserModel.findOne.mockResolvedValue({ email: userData.email });

      await expect(userService.createUser(userData)).rejects.toThrow('Email already exists');
      expect(mockUserModel.create).not.toHaveBeenCalled();
    });

    test('should handle email service failure gracefully', async () => {
      const userData = {
        name: 'Test User',
        email: 'test@example.com',
        password: 'password123'
      };

      const expectedUser = {
        _id: 'user123',
        name: userData.name,
        email: userData.email,
        role: 'user'
      };

      mockUserModel.findOne.mockResolvedValue(null);
      mockUserModel.create.mockResolvedValue(expectedUser);
      mockEmailService.sendWelcomeEmail.mockRejectedValue(new Error('Email service down'));

      const result = await userService.createUser(userData);

      expect(result).toBeDefined();
      expect(result.email).toBe(userData.email);
      // User should still be created even if email fails
    });
  });

  describe('authenticateUser', () => {
    test('should authenticate with valid credentials', async () => {
      const email = 'test@example.com';
      const password = 'plainpassword';
      const hashedPassword = 'hashedpassword';

      const mockUser = {
        _id: 'user123',
        email,
        password: hashedPassword,
        comparePassword: jest.fn().mockResolvedValue(true)
      };

      mockUserModel.findOne.mockResolvedValue(mockUser);

      const result = await userService.authenticateUser(email, password);

      expect(mockUserModel.findOne).toHaveBeenCalledWith({ email });
      expect(mockUser.comparePassword).toHaveBeenCalledWith(password);
      expect(result).toEqual({
        id: mockUser._id,
        email: mockUser.email
      });
    });

    test('should reject invalid credentials', async () => {
      const email = 'test@example.com';
      const password = 'wrongpassword';

      const mockUser = {
        _id: 'user123',
        email,
        password: 'hashedpassword',
        comparePassword: jest.fn().mockResolvedValue(false)
      };

      mockUserModel.findOne.mockResolvedValue(mockUser);

      await expect(userService.authenticateUser(email, password))
        .rejects.toThrow('Invalid credentials');
    });

    test('should handle user not found', async () => {
      const email = 'nonexistent@example.com';
      const password = 'password123';

      mockUserModel.findOne.mockResolvedValue(null);

      await expect(userService.authenticateUser(email, password))
        .rejects.toThrow('Invalid credentials');
    });
  });
});
```

## Mocking APIs for Testing

### Why Mock APIs?

- **Isolation**: Test your code without depending on external services
- **Speed**: Avoid network calls and external service latency
- **Reliability**: Eliminate flakiness from external service issues
- **Cost**: Avoid charges from third-party API calls during testing
- **Edge Cases**: Simulate various response scenarios easily

### Mocking Strategies

#### 1. Mock Server with Nock

```javascript
// Mock API calls using Nock
const nock = require('nock');

describe('External API Integration with Mocking', () => {
  beforeEach(() => {
    // Clear all interceptors before each test
    nock.cleanAll();
  });

  afterEach(() => {
    // Verify all mocks were called
    expect(nock.isDone()).toBe(true);
  });

  test('should handle successful API response', async () => {
    // Mock external API response
    const scope = nock('https://api.external-service.com')
      .get('/users/123')
      .reply(200, {
        id: 123,
        name: 'External User',
        email: 'external@example.com'
      });

    const result = await externalUserService.getUserData(123);

    expect(result).toEqual({
      id: 123,
      name: 'External User',
      email: 'external@example.com'
    });

    expect(scope.isDone()).toBe(true);
  });

  test('should handle API error responses', async () => {
    // Mock API error
    nock('https://api.external-service.com')
      .get('/users/404')
      .reply(404, {
        error: 'User not found'
      });

    await expect(externalUserService.getUserData(404))
      .rejects.toThrow('User not found');
  });

  test('should handle network timeouts', async () => {
    // Mock network timeout
    nock('https://api.external-service.com')
      .get('/users/timeout')
      .delay(5000) // 5 second delay
      .reply(200, { id: 999 });

    // Set short timeout for the test
    const timeoutPromise = new Promise((_, reject) =>
      setTimeout(() => reject(new Error('Request timeout')), 1000)
    );

    await expect(Promise.race([
      externalUserService.getUserData(999),
      timeoutPromise
    ])).rejects.toThrow('Request timeout');
  });

  test('should handle rate limiting', async () => {
    // Mock rate limiting response
    nock('https://api.external-service.com')
      .get('/users/rate-limited')
      .reply(429, {
        error: 'Rate limit exceeded',
        retryAfter: 60
      });

    await expect(externalUserService.getUserData('rate-limited'))
      .rejects.toThrow('Rate limit exceeded');
  });
});
```

#### 2. Mock Factory Pattern

```javascript
// Mock Factory for creating test data
class MockFactory {
  static createUser(overrides = {}) {
    return {
      id: 'user123',
      name: 'Test User',
      email: 'test@example.com',
      role: 'user',
      createdAt: new Date().toISOString(),
      ...overrides
    };
  }

  static createOrder(overrides = {}) {
    return {
      id: 'order123',
      userId: 'user123',
      items: [
        { productId: 'prod1', quantity: 2, price: 29.99 },
        { productId: 'prod2', quantity: 1, price: 49.99 }
      ],
      total: 109.97,
      status: 'pending',
      createdAt: new Date().toISOString(),
      ...overrides
    };
  }

  static createApiResponse(overrides = {}) {
    return {
      success: true,
      data: null,
      message: 'Operation successful',
      timestamp: new Date().toISOString(),
      ...overrides
    };
  }

  static createPaginatedResponse(data, page = 1, limit = 10) {
    return {
      data,
      pagination: {
        page,
        limit,
        total: data.length,
        totalPages: Math.ceil(data.length / limit)
      }
    };
  }
}

// Usage in tests
describe('Service with Mock Data', () => {
  test('should process user with mock data', async () => {
    const mockUser = MockFactory.createUser({
      role: 'admin',
      email: 'admin@example.com'
    });

    const result = await userService.processUser(mockUser);

    expect(result.isAdmin).toBe(true);
    expect(result.emailDomain).toBe('example.com');
  });

  test('should handle paginated results', async () => {
    const mockUsers = Array.from({ length: 25 }, (_, i) =>
      MockFactory.createUser({ id: `user${i}` })
    );

    const paginatedResult = MockFactory.createPaginatedResponse(mockUsers, 1, 10);

    expect(paginatedResult.data).toHaveLength(10);
    expect(paginatedResult.pagination.total).toBe(25);
    expect(paginatedResult.pagination.totalPages).toBe(3);
  });
});
```

#### 3. WireMock for Complex Scenarios

```javascript
// WireMock integration for comprehensive API mocking
const wiremock = require('wiremock');

class WireMockHelper {
  constructor() {
    this.client = wiremock('http://localhost:8080');
  }

  async setupPaymentMocks() {
    // Successful payment
    await this.client.stubFor({
      request: {
        method: 'POST',
        url: '/api/payments/charge',
        headers: {
          'Content-Type': { equalTo: 'application/json' }
        },
        bodyPatterns: [{
          matchesJsonPath: '$[?(@.amount == 2999)]'
        }]
      },
      response: {
        status: 200,
        headers: { 'Content-Type': 'application/json' },
        jsonBody: {
          id: 'ch_success123',
          status: 'succeeded',
          amount: 2999,
          currency: 'usd'
        }
      }
    });

    // Failed payment
    await this.client.stubFor({
      request: {
        method: 'POST',
        url: '/api/payments/charge',
        headers: {
          'Content-Type': { equalTo: 'application/json' }
        },
        bodyPatterns: [{
          matchesJsonPath: '$[?(@.amount == 9999)]'
        }]
      },
      response: {
        status: 402,
        headers: { 'Content-Type': 'application/json' },
        jsonBody: {
          error: 'Payment declined',
          code: 'card_declined'
        }
      }
    });

    // Network timeout simulation
    await this.client.stubFor({
      request: {
        method: 'POST',
        url: '/api/payments/charge',
        headers: {
          'Content-Type': { equalTo: 'application/json' }
        },
        bodyPatterns: [{
          matchesJsonPath: '$[?(@.amount == 5000)]'
        }]
      },
      response: {
        fixedDelayMilliseconds: 10000, // 10 second delay
        status: 200
      }
    });
  }

  async reset() {
    await this.client.reset();
  }
}

// Test usage
describe('Payment Integration with WireMock', () => {
  let wiremockHelper;

  beforeAll(async () => {
    wiremockHelper = new WireMockHelper();
    await wiremockHelper.setupPaymentMocks();
  });

  afterAll(async () => {
    await wiremockHelper.reset();
  });

  test('should process successful payment', async () => {
    const paymentData = {
      amount: 2999,
      currency: 'usd',
      source: 'tok_test123'
    };

    const result = await paymentService.processPayment(paymentData);

    expect(result.status).toBe('succeeded');
    expect(result.chargeId).toBe('ch_success123');
  });

  test('should handle declined payment', async () => {
    const paymentData = {
      amount: 9999,
      currency: 'usd',
      source: 'tok_declined'
    };

    await expect(paymentService.processPayment(paymentData))
      .rejects.toThrow('Payment declined');
  });
});
```

## Contract Testing

### What is Contract Testing?

Contract testing ensures that API providers and consumers can communicate effectively. It validates that the consumer's expectations match the provider's capabilities without testing the entire system.

### Contract Testing with Pact

#### 1. Consumer Contract Testing

```javascript
// Consumer Contract Tests
const { Pact } = require('@pact-foundation/pact');
const path = require('path');
const { getUserById } = require('./userClient');

describe('User API Consumer Contract', () => {
  const provider = new Pact({
    consumer: 'UserWebApp',
    provider: 'UserAPI',
    port: 1234,
    log: path.resolve(process.cwd(), 'logs', 'pact.log'),
    dir: path.resolve(process.cwd(), 'pacts'),
    logLevel: 'INFO'
  });

  beforeAll(async () => {
    await provider.setup();
  });

  afterAll(async () => {
    await provider.finalize();
  });

  describe('Get User by ID', () => {
    beforeEach(async () => {
      await provider.addInteraction({
        state: 'user with ID 123 exists',
        uponReceiving: 'a request for user 123',
        withRequest: {
          method: 'GET',
          path: '/api/users/123',
          headers: {
            'Accept': 'application/json'
          }
        },
        willRespondWith: {
          status: 200,
          headers: {
            'Content-Type': 'application/json; charset=utf-8'
          },
          body: {
            id: 123,
            name: 'John Doe',
            email: 'john@example.com',
            role: 'user',
            createdAt: Pact.like('2023-01-01T00:00:00.000Z')
          }
        }
      });
    });

    test('should return user data', async () => {
      const user = await getUserById(123);

      expect(user.id).toBe(123);
      expect(user.name).toBe('John Doe');
      expect(user.email).toBe('john@example.com');
      expect(user.role).toBe('user');
      expect(user.createdAt).toBeDefined();
    });

    afterEach(async () => {
      await provider.verify();
    });
  });

  describe('User Not Found', () => {
    beforeEach(async () => {
      await provider.addInteraction({
        state: 'user with ID 999 does not exist',
        uponReceiving: 'a request for non-existent user 999',
        withRequest: {
          method: 'GET',
          path: '/api/users/999',
          headers: {
            'Accept': 'application/json'
          }
        },
        willRespondWith: {
          status: 404,
          headers: {
            'Content-Type': 'application/json; charset=utf-8'
          },
          body: {
            error: 'User not found'
          }
        }
      });
    });

    test('should handle user not found', async () => {
      await expect(getUserById(999))
        .rejects.toThrow('User not found');
    });

    afterEach(async () => {
      await provider.verify();
    });
  });

  describe('Create User', () => {
    beforeEach(async () => {
      await provider.addInteraction({
        uponReceiving: 'a request to create a new user',
        withRequest: {
          method: 'POST',
          path: '/api/users',
          headers: {
            'Content-Type': 'application/json',
            'Authorization': Pact.like('Bearer token123')
          },
          body: {
            name: 'New User',
            email: 'newuser@example.com',
            password: 'password123'
          }
        },
        willRespondWith: {
          status: 201,
          headers: {
            'Content-Type': 'application/json; charset=utf-8'
          },
          body: Pact.like({
            id: 456,
            name: 'New User',
            email: 'newuser@example.com',
            role: 'user',
            createdAt: '2023-12-01T10:30:00.000Z'
          })
        }
      });
    });

    test('should create new user', async () => {
      const newUser = {
        name: 'New User',
        email: 'newuser@example.com',
        password: 'password123'
      };

      const createdUser = await createUser(newUser);

      expect(createdUser.id).toBe(456);
      expect(createdUser.name).toBe('New User');
      expect(createdUser.email).toBe('newuser@example.com');
      expect(createdUser.role).toBe('user');
      expect(createdUser.createdAt).toBeDefined();
    });

    afterEach(async () => {
      await provider.verify();
    });
  });
});
```

#### 2. Provider Contract Testing

```javascript
// Provider Contract Tests
const { Verifier } = require('@pact-foundation/pact');
const path = require('path');
const app = require('../app'); // Your Express app

describe('User API Provider Contract', () => {
  const SERVER_URL = 'http://localhost:3001';

  beforeAll(async () => {
    // Start the API server for testing
    await new Promise(resolve => {
      server = app.listen(3001, () => {
        console.log('API server running on port 3001');
        resolve();
      });
    });
  });

  afterAll(async () => {
    if (server) {
      await new Promise(resolve => server.close(resolve));
    }
  });

  it('validates the expectations of UserWebApp', () => {
    let token = 'Bearer valid-token-123';

    const stateHandlers = {
      'user with ID 123 exists': () => {
        // Setup database state for test
        return require('../models/User').create({
          id: 123,
          name: 'John Doe',
          email: 'john@example.com',
          role: 'user'
        });
      },
      'user with ID 999 does not exist': () => {
        // Ensure user 999 doesn't exist
        return require('../models/User').deleteOne({ id: 999 });
      }
    };

    const requestFilters = {
      // Add authentication headers to all requests
      headers: {
        'Authorization': token
      }
    };

    return new Verifier({
      provider: 'UserAPI',
      providerBaseUrl: SERVER_URL,
      pactUrls: [
        path.resolve(process.cwd(), 'pacts', 'userwebapp-userapi.json')
      ],
      stateHandlers,
      requestFilters,
      logLevel: 'INFO'
    }).verifyProvider()
      .then(output => {
        console.log('Pact Verification Complete!');
        console.log(output);
      });
  });
});
```

#### 3. Contract Testing with Dredd

```yaml
# dredd.yml (Dredd configuration)
server: http://localhost:3000
endpoint: /api
template: blueprint
options:
  names: true
  sorted: true
  reporter: ['apiary', 'junit']
  'output': ['test-reports/api-dredd.xml']
  'hookfiles': ['test/hooks.js']
  'header': 'Authorization: Bearer test-token-123'
```

```javascript
// test/hooks.js (Dredd hooks)
const { spawn } = require('child_process');
const mongoose = require('mongoose');

let serverProcess;

exports.beforeAll = function (done) {
  // Start API server
  serverProcess = spawn('npm', ['start'], {
    stdio: 'inherit'
  });

  // Wait for server to start
  setTimeout(() => {
    done();
  }, 3000);
};

exports.afterAll = function (done) {
  if (serverProcess) {
    serverProcess.kill();
  }
  done();
};

exports.before = function (transaction) {
  // Setup test data before each request
  if (transaction.name === 'User Collection > Get User > *') {
    transaction.id = '123';
    transaction.expected = {
      statusCode: 200,
      headers: {
        'Content-Type': 'application/json'
      },
      body: {
        id: 123,
        name: 'Test User',
        email: 'test@example.com'
      }
    };
  }
};

exports.beforeEach = function (transaction) {
  // Database cleanup and setup
  return mongoose.connection.db.dropDatabase()
    .then(() => {
      // Setup required test data
      return mongoose.model('User').create({
        id: 123,
        name: 'Test User',
        email: 'test@example.com'
      });
    });
};
```

## Best Practices for API Testing

### 1. Test Organization

```javascript
// Test organization structure
describe('API Test Suite', () => {
  // Test Setup
  beforeAll(() => { /* Global setup */ });
  afterAll(() => { /* Global cleanup */ });

  beforeEach(() => { /* Test-specific setup */ });
  afterEach(() => { /* Test-specific cleanup */ });

  // Group tests by functionality
  describe('Authentication', () => {
    test('should authenticate with valid credentials');
    test('should reject invalid credentials');
  });

  describe('User Management', () => {
    describe('GET /users', () => {
      test('should return users list');
      test('should handle pagination');
    });

    describe('POST /users', () => {
      test('should create new user');
      test('should validate input');
    });
  });
});
```

### 2. Test Data Management

```javascript
// Test data factories and fixtures
class TestDataFactory {
  static fixtures = {
    users: [],
    orders: [],
    products: []
  };

  static createFixture(type, count = 1, overrides = {}) {
    const fixtures = Array.from({ length: count }, (_, i) =>
      this.create(type, { ...overrides, id: `${type}_${i}` })
    );

    this.fixtures[type].push(...fixtures);
    return count === 1 ? fixtures[0] : fixtures;
  }

  static cleanupFixture(type) {
    this.fixtures[type] = [];
  }
}
```

### 3. Environment Configuration

```javascript
// Test environment configuration
const testConfig = {
  development: {
    apiUrl: 'http://localhost:3000',
    databaseUrl: 'mongodb://localhost:27017/api_test_dev',
    timeout: 5000
  },

  staging: {
    apiUrl: 'https://api-staging.example.com',
    databaseUrl: process.env.STAGING_DB_URL,
    timeout: 10000
  },

  production: {
    apiUrl: 'https://api.example.com',
    timeout: 15000
  }
};

const config = testConfig[process.env.NODE_ENV || 'development'];
```

## Interview Questions

### Basic Questions

1. **What is API testing and why is it important?**
   - API testing verifies functionality, performance, security, and reliability
   - Catches bugs early, validates business logic, ensures integration works

2. **What's the difference between unit, integration, and functional testing?**
   - Unit: Tests individual components in isolation
   - Integration: Tests interaction between components
   - Functional: Tests complete functionality from user perspective

3. **What are HTTP status codes you commonly test for?**
   - 200 (OK), 201 (Created), 204 (No Content)
   - 400 (Bad Request), 401 (Unauthorized), 403 (Forbidden), 404 (Not Found)
   - 500 (Internal Server Error), 502 (Bad Gateway), 503 (Service Unavailable)

### Intermediate Questions

4. **How do you test API authentication and authorization?**
   - Test with valid/invalid tokens
   - Verify role-based access controls
   - Test expired tokens and token refresh

5. **What is contract testing and when should you use it?**
   - Verifies API provider meets consumer expectations
   - Used in microservices architecture
   - Prevents breaking changes between services

6. **How do you perform load testing for APIs?**
   - Tools: K6, Artillery, Apache Bench
   - Metrics: Response time, throughput, error rates
   - Scenarios: Normal, peak, stress testing

### Advanced Questions

7. **How do you handle test data management in API testing?**
   - Use factories and fixtures
   - Implement proper cleanup
   - Use test databases with proper isolation

8. **What's the difference between mocking and stubbing in API tests?**
   - Mocking: Verifies behavior and interactions
   - Stubbing: Provides canned responses
   - Choose based on test objectives

9. **How do you test asynchronous API operations?**
   - Use polling or webhooks to check completion
   - Implement proper timeout handling
   - Test error scenarios and retries

### Scenario-Based Questions

10. **How would you test a file upload API?**
    - Test with various file types and sizes
    - Verify validation and error handling
    - Check storage and processing

11. **How do you test APIs that depend on third-party services?**
    - Mock external service responses
    - Test with various response scenarios
    - Implement circuit breaker testing

12. **How would you implement API testing in a CI/CD pipeline?**
    - Automated test execution
    - Parallel test execution
    - Test reporting and failure notifications

## Summary

API testing is crucial for building reliable, secure, and performant APIs. A comprehensive testing strategy includes:

- **Load Testing**: Validate performance under expected loads
- **Functional Testing**: Verify business logic and requirements
- **Integration Testing**: Test component interactions
- **Unit Testing**: Test individual components in isolation
- **Mocking**: Isolate tests from external dependencies
- **Contract Testing**: Ensure provider/consumer compatibility

Key success factors:
- Start testing early in development
- Automate tests and integrate with CI/CD
- Use appropriate tools for different test types
- Maintain test data properly
- Monitor test coverage and effectiveness

Remember: Good API testing catches bugs before users do, ensures reliability, and provides confidence in system changes.