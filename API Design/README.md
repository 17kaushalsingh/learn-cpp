# API Design 🚀

## Overview 📋

**This folder contains comprehensive materials for API design, covering REST APIs, GraphQL, authentication, error handling, and best practices. Essential for backend development and system design interviews.**

## Complete Learning Path 🛤️

### **1. Learn the Basics** 📚

#### **Introduction** 🎯
- **What are APIs**: Application Programming Interfaces - contracts for software communication
- **API Types**: Web APIs, Library APIs, OS APIs
- **API Evolution**: From RPC to REST to GraphQL

#### **HTTP** 🌐
- **HTTP Protocol**: Foundation of web communication
- **Request-Response Cycle**: Client-server communication pattern
- **Stateless Nature**: Each request is independent

#### **HTTP Versions** 📈
```
HTTP/1.0 (1996): Basic HTTP, one connection per request
HTTP/1.1 (1997): Persistent connections, pipelining, host headers
HTTP/2 (2015): Multiplexing, header compression, binary protocol
HTTP/3 (2022): QUIC transport, improved performance
```

#### **HTTP Methods** 🔧
| **Method** | **Purpose** | **Idempotent** | **Safe** | **Cacheable** |
|------------|-------------|----------------|----------|--------------|
| **GET** | Retrieve data | Yes | Yes | Yes |
| **POST** | Create data | No | No | Conditional |
| **PUT** | Update/replace data | Yes | No | No |
| **PATCH** | Partial update | No | No | No |
| **DELETE** | Remove data | Yes | No | No |
| **HEAD** | Get headers only | Yes | Yes | Yes |
| **OPTIONS** | Get allowed methods | Yes | Yes | No |

#### **HTTP Status Codes** 📊
```
✅ Success (2xx): 200 OK, 201 Created, 204 No Content
🔄 Redirection (3xx): 301 Moved Permanently, 304 Not Modified
❌ Client Error (4xx): 400 Bad Request, 401 Unauthorized, 404 Not Found
⚠️ Server Error (5xx): 500 Internal Server Error, 503 Service Unavailable
```

#### **HTTP Headers** 📋
**Request Headers**:
- `Authorization`: Authentication credentials
- `Content-Type`: Media type of request body
- `Accept`: Media types client can handle
- `User-Agent`: Client identification

**Response Headers**:
- `Content-Type`: Media type of response body
- `Cache-Control`: Caching directives
- `Set-Cookie`: Cookie setting
- `Location`: URL for redirection

#### **Cookies** 🍪
```
Purpose: Server-side state management
Components: Name-value pair, attributes (expires, domain, path)
Security: HttpOnly, Secure, SameSite attributes
```

#### **CORS** 🌐
**Cross-Origin Resource Sharing**:
```
Origin Header: http://example.com
Access-Control-Allow-Origin: *
Access-Control-Allow-Methods: GET, POST, PUT
Access-Control-Allow-Headers: Content-Type, Authorization
```

#### **HTTP Caching** 💾
```
Cache-Control: max-age=3600, public
ETag: "xyz123"
Last-Modified: Wed, 21 Oct 2015 07:28:00 GMT
```

#### **Basics** 🔧
- **URL Structure**: `https://api.example.com/v1/users/123/posts`
- **Query Parameters**: `?page=2&limit=20&sort=desc`
- **Path Parameters**: `/users/{id}/posts/{post_id}`
- **Content Negotiation**: `Accept: application/json` vs `Accept: application/xml`

#### **Understand TCP/IP** 🌐
- **TCP**: Reliable, connection-oriented protocol
- **IP**: Best-effort, connectionless protocol
- **DNS**: Domain Name System for name resolution

### **2. Different API Styles** 🎨

#### **RESTful APIs** 🏗️
**Characteristics**:
- **Resource-based**: `/users`, `/posts`, `/comments`
- **Stateless**: Each request contains all context
- **HTTP methods**: Proper GET, POST, PUT, DELETE usage
- **Uniform interface**: Consistent patterns

#### **Simple JSON APIs** 📄
```
Example: Simple POST API
POST /api/login
{
  "email": "user@example.com",
  "password": "secret123"
}

Response:
{
  "success": true,
  "token": "jwt_token_here",
  "user": { "id": 123, "name": "John" }
}
```

#### **SOAP APIs** 🧼
**Characteristics**:
- **XML-based**: Structured message format
- **WSDL**: Web Services Description Language
- **Strict contracts**: Defined input/output schemas
- **Enterprise features**: Built-in security, transactions

#### **GraphQL APIs** 🔍
**Characteristics**:
- **Single endpoint**: `/graphql`
- **Query language**: Clients request exactly what they need
- **Strong typing**: Schema definition
- **Real-time**: Subscriptions support

#### **gRPC APIs** 🚀
**Characteristics**:
- **Protocol Buffers**: Efficient serialization
- **HTTP/2**: Multiplexed connections
- **Strong contracts**: .proto service definitions
- **High performance**: Binary protocol

### **3. Building JSON / RESTful APIs** 🔧

#### **Handling CRUD Operations** 📋
```
CREATE: POST /api/users
READ:   GET /api/users/{id}
UPDATE: PUT /api/users/{id}
DELETE: DELETE /api/users/{id}

List:   GET /api/users?page=1&limit=20
Search: GET /api/users?search=john
```

#### **Versioning Strategies** 📊
```
1. URL Versioning: /api/v1/users
2. Header Versioning: Accept: application/vnd.api+json;version=1
3. Query Parameter: /api/users?version=1
4. Domain Versioning: v1.api.example.com/users
```

#### **URI Design** 🏗️
```
Good: /api/v1/users/{user_id}/posts/{post_id}/comments
Bad: /api/v1/getUserPostsComments?user=123&post=456

Principles:
- Use nouns, not verbs
- Plural resource names
- Hierarchical relationships
- Consistent patterns
```

#### **REST Principles** 🎯
1. **Client-Server**: Separation of concerns
2. **Stateless**: No client context stored on server
3. **Cacheable**: Responses must indicate cacheability
4. **Uniform Interface**: Standardized methods and conventions
5. **Layered System**: Architecture with multiple layers
6. **Code on Demand**: Optional server-side logic

#### **Pagination** 📄
```
Offset-based: /api/users?page=2&limit=20
Cursor-based: /api/users?after=cursor123
Keyset-based: /api/users?since=2023-01-01

Response structure:
{
  "data": [...],
  "pagination": {
    "page": 2,
    "limit": 20,
    "total": 150,
    "hasNext": true
  }
}
```

#### **Rate Limiting** 🚦
```
Headers:
X-RateLimit-Limit: 1000
X-RateLimit-Remaining: 999
X-RateLimit-Reset: 1640995200

Response: 429 Too Many Requests
{
  "error": "Rate limit exceeded",
  "retryAfter": 60
}
```

#### **Idempotency** 🔄
```
Definition: Same request produces same result
Methods: GET, PUT, DELETE are idempotent
POST is not idempotent by default

Implementation:
POST /api/process
X-Idempotency-Key: unique-key-for-this-operation
```

#### **HATEOAS** 🔗
**Hypermedia as the Engine of Application State**:
```json
{
  "user": {
    "id": 123,
    "name": "John",
    "_links": {
      "self": { "href": "/api/users/123" },
      "posts": { "href": "/api/users/123/posts" },
      "profile": { "href": "/api/users/123/profile" }
    }
  }
}
```

#### **Error Handling** ❌
**RFC 7807 - Problem Details for APIs**:
```json
{
  "type": "https://example.com/errors/validation",
  "title": "Validation Failed",
  "status": 400,
  "detail": "Invalid email format",
  "instance": "/api/users/123",
  "errors": [
    {
      "field": "email",
      "message": "Invalid email format"
    }
  ]
}
```

### **4. Authentication Methods** 🔐

#### **Basic Auth** 🔑
```
Authorization: Basic base64(username:password)
Pros: Simple, widely supported
Cons: Credentials sent with each request
```

#### **Token Based Auth** 🎫
```
Process:
1. User login with credentials
2. Server validates and returns token
3. Client stores token and sends in subsequent requests

Authorization: Bearer <token>
```

#### **JWT (JSON Web Tokens)** 📜
```
Structure: Header.Payload.Signature
{
  "header": {"alg": "HS256", "typ": "JWT"},
  "payload": {"sub": "123", "exp": 1640995200},
  "signature": "HMACSHA256(base64UrlEncode(header) + "." + base64UrlEncode(payload), secret)"
}
```

#### **OAuth 2.0** 🔑
```
Authorization Flow:
1. Authorization Request: /authorize?response_type=code&client_id=...
2. User Authorization: User grants permission
3. Authorization Code: Redirect with code
4. Token Exchange: Exchange code for access token
5. Access Token: Make API requests
```

#### **Session Based Auth** 📋
```
Process:
1. Login creates session on server
2. Session ID stored in cookie
3. Server validates session on each request
4. Session stores user state on server
```

### **5. Authorization Methods** 🛡️

#### **RBAC (Role-Based Access Control)** 👥
```
Users → Roles → Permissions
Example: John → Admin → [read, write, delete]
```

#### **ABAC (Attribute-Based Access Control)** 📊
```
Access decisions based on:
- User attributes (age, department)
- Resource attributes (sensitivity level)
- Environmental attributes (time, location)
```

#### **DAC (Discretionary Access Control)** 🔒
```
Resource owners define access policies
Example: File owner sets read/write permissions for other users
```

#### **MAC (Mandatory Access Control)** 🏛️
```
System-enforced access control
Example: Government classified documents with security clearances
```

#### **PBAC (Policy-Based Access Control)** 📋
```
Access decisions based on predefined policies
Example: If user is manager AND budget > $1000 THEN allow approval
```

#### **ReBAC (Relationship-Based Access Control)** 🔗
```
Access based on relationships between entities
Example: Owner can edit document, collaborators can view
```

### **6. API Keys & Management** 🔑

#### **API Key Types**:
- **Public Keys**: For client identification
- **Secret Keys**: For server authentication
- **Subscription Keys**: For paid API services

#### **Key Management**:
```
Generation: Cryptographically secure random strings
Storage: Encrypted at rest
Rotation: Regular key replacement policies
Revocation: Immediate invalidation capability
```

### **7. API Documentation Tools** 📚

#### **Swagger / Open API** 📖
```yaml
openapi: 3.0.0
info:
  title: User API
  version: 1.0.0
paths:
  /users:
    get:
      summary: Get all users
      responses:
        '200':
          description: Successful response
```

#### **Readme.com** 📖
**Interactive API Documentation**
- Hosted documentation platform
- API reference generation
- Interactive API explorer
- Code examples in multiple languages

#### **Stoplight** 🛑
**Visual API Design**
- Visual API designer
- Automated testing
- Mock server generation
- Team collaboration features

#### **Postman** 📮
**API Testing Platform**
- Request collection and organization
- Automated testing
- Mock server
- API documentation sharing

### **8. API Security** 🔒

#### **Best Practices**:
- **HTTPS Only**: Encrypt all communications
- **Input Validation**: Validate all inputs
- **Rate Limiting**: Prevent abuse
- **Authentication**: Strong auth mechanisms
- **Authorization**: Proper access controls
- **Logging**: Comprehensive audit trails
- **Regular Security Audits**: Periodic security reviews

#### **Common Vulnerabilities**:
- **SQL Injection**: Parameterized queries
- **XSS**: Input sanitization
- **CSRF**: Anti-CSRF tokens
- **Authentication Bypass**: Strong password policies
- **Data Exposure**: Minimize data in responses
- **Rate Limiting Bypass**: Implement robust limiting

### **9. API Performance** ⚡

#### **Performance Metrics** 📊
```
Response Time: Time to send response
Throughput: Requests per second
Error Rate: Percentage of failed requests
Availability: Uptime percentage
Latency: Network delay + processing time
```

#### **Caching Strategies** 💾
```
Client Caching: Browser, mobile app
Server Caching: Redis, Memcached
CDN Caching: Edge location caching
Database Caching: Query result caching
```

#### **Load Balancing** ⚖️
```
Algorithms: Round Robin, Least Connections, IP Hash
Types: L4 (Transport), L7 (Application)
Health Checks: Regular endpoint monitoring
Failover: Automatic traffic redirection
```

#### **Rate Limiting / Throttling** 🚦
```
Types:
- Fixed Window: X requests per time period
- Sliding Window: Rolling time period
- Token Bucket: Burst capacity + sustained rate
- Leaky Bucket: Constant output rate
```

#### **Profiling and Monitoring** 📈
```
Tools: APM solutions, custom metrics
Metrics: Response time, error rates, resource usage
Alerts: Threshold-based notifications
Dashboards: Real-time visualization
```

#### **Performance Testing** 🧪
```
Types: Load, Stress, Endurance, Spike
Tools: JMeter, K6, Gatling
Metrics: Response times, throughput, error rates
Scenarios: Realistic usage patterns
```

#### **Error Handling / Retries** 🔄
```
Retry Patterns:
- Exponential Backoff: Increasing delays
- Circuit Breaker: Fail fast when downstream is failing
- Dead Letter Queue: Failed message storage
- Idempotency: Safe retry mechanisms
```

#### **Best Practices** 🎯
```
- Optimize Database Queries: Indexes, query optimization
- Use Compression: Gzip, Brotli
- Implement Connection Pooling: Reuse connections
- Monitor Performance: Regular performance audits
- Use CDN: Geographic distribution
- Implement Caching: Multiple cache layers
```

---

## File Structure 📂

```
API Design/
├── README.md                     # This file
├── 1_Learn_The_Basics/
│   ├── HTTP_Fundamentals.md      # HTTP protocol and versions
│   ├── HTTP_Methods_Codes.md      # Methods, status codes, headers
│   ├── Cookies_CORS_Caching.md    # Web security and performance
│   └── URL_Design_Basics.md       # URL structure and parameters
├── 2_API_Styles/
│   ├── RESTful_APIs.md           # REST principles and design
│   ├── Simple_JSON_APIs.md       # Basic JSON API patterns
│   ├── SOAP_APIs.md              # SOAP web services
│   ├── GraphQL_APIs.md           # GraphQL fundamentals
│   └── gRPC_APIs.md               # gRPC and Protocol Buffers
├── 3_Building_RESTful_APIs/
│   ├── CRUD_Operations.md        # Create, Read, Update, Delete
│   ├── Versioning_Strategies.md   # API versioning approaches
│   ├── URI_Design_Principles.md  # Resource naming and structure
│   ├── REST_Principles.md         # REST architectural constraints
│   ├── Pagination_Patterns.md    # Data pagination techniques
│   ├── Rate_Limiting.md           # Throttling and rate limiting
│   ├── Idempotency.md             # Idempotent API design
│   ├── HATEOAS.md                 # Hypermedia APIs
│   └── RFC_7807_Error_Handling.md # Standardized error responses
├── 4_Authentication_Methods/
│   ├── Basic_Authentication.md    # Basic authentication scheme
│   ├── Token_Based_Auth.md        # Token-based authentication
│   ├── JWT_Authentication.md      # JSON Web Tokens
│   ├── OAuth_2.0_Flows.md         # OAuth 2.0 authorization
│   └── Session_Based_Auth.md       # Session management
├── 5_Authorization_Methods/
│   ├── RBAC_Authorization.md       # Role-based access control
│   ├── ABAC_Authorization.md       # Attribute-based access control
│   ├── DAC_Authorization.md        # Discretionary access control
│   ├── MAC_Authorization.md        # Mandatory access control
│   ├── PBAC_Authorization.md       # Policy-based access control
│   └── ReBAC_Authorization.md       # Relationship-based access control
├── 6_API_Keys_Management/
│   └── API_Key_Security.md         # Key generation, rotation, revocation
├── 7_API_Documentation_Tools/
│   ├── Swagger_OpenAPI.md          # API specification standards
│   ├── Readme_Documentation.md     # Interactive documentation
│   ├── Stoplight_Platform.md       # Visual API design
│   └── Postman_Workspace.md        # API testing and documentation
├── 8_API_Security/
│   ├── Security_Best_Practices.md  # Comprehensive security guide
│   └── Common_Vulnerabilities.md    # Security threats and mitigations
├── 9_API_Performance/
│   ├── Performance_Metrics.md      # Measuring API performance
│   ├── Caching_Strategies.md       # Multi-level caching approaches
│   ├── Load_Balancing.md           # Traffic distribution strategies
│   ├── Rate_Limiting_Throttling.md # Advanced throttling techniques
│   ├── Profiling_Monitoring.md     # Performance analysis tools
│   ├── Performance_Testing.md       # Load and stress testing
│   ├── Error_Retries.md            # Retry patterns and error handling
│   └── Performance_Best_Practices.md # Optimization techniques
└── Examples/
    ├── E_Commerce_API_Design.md    # Complete e-commerce API example
    ├── Social_Media_API.md         # Social platform API design
    ├── File_Upload_API.md          # File handling and storage
    └── WebSocket_Realtime_API.md    # Real-time communication APIs
```

---

## Success Tips 🎯

### **Before API Design**
- **Understand requirements** thoroughly
- **Choose appropriate API style** (REST, GraphQL, gRPC)
- **Plan for scalability** from the start
- **Consider security requirements** early

### **During Development**
- **Follow consistent patterns** across all endpoints
- **Implement comprehensive error handling**
- **Add monitoring and logging** from day one
- **Write thorough documentation**

### **For Interview Preparation**
- **Practice designing APIs** for different scenarios
- **Understand trade-offs** between different approaches
- **Be ready to explain** your design decisions
- **Know security best practices** thoroughly
- **Understand performance** implications

---

**Remember**: Great API design is about creating interfaces that are intuitive, consistent, secure, and performant. Focus on developer experience while maintaining technical excellence! 🚀

**Last Updated**: November 2025