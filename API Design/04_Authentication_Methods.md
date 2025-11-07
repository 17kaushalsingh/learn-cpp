# Chapter 04: Authentication Methods

## Introduction

**📌 Authentication**: The process of verifying who someone is. Think of it like showing your ID card - it proves you are who you claim to be.

**📌 Authorization**: The process of verifying what someone is allowed to do. Think of it like having a keycard that opens specific doors.

This chapter covers different ways to prove user identity in APIs.

---

## Basic Authentication

### What is Basic Authentication?

**Basic Auth**: The simplest authentication method where credentials are sent as a Base64-encoded string in HTTP headers.

**📌 Think of it like this**: Writing your username and password on a postcard and sending it through the mail - it works, but it's not very secure.

### How Basic Auth Works

```mermaid
sequenceDiagram
    participant Client as Client App
    participant Server as API Server

    Client->>Server: Request without credentials
    Server->>Client: 401 Unauthorized + "Please authenticate"
    Client->>Server: Request with Authorization header
    Server->>Client: 200 OK + Requested data
```

### Basic Auth Request Format

```http
GET /api/users HTTP/1.1
Host: api.example.com
Authorization: Basic dXNlcm5hbWU6cGFzc3dvcmQ=
```

**Encoding Process:**
- Original: `username:password`
- Base64 encoded: `dXNlcm5hbWU6cGFzc3dvcmQ=`
- **Important**: This is encoding, NOT encryption!

### When to Use Basic Auth

✅ **Good for:**
- Internal APIs
- Development and testing
- Simple server-to-server communication
- When combined with HTTPS

❌ **Avoid for:**
- Production public APIs
- Mobile applications
- Sensitive data access
- When credentials need to be shared

### Security Considerations

| Issue | Risk | Solution |
|-------|------|----------|
| **Plain credentials** | High | Always use HTTPS |
| **Base64 decoding** | Easy to decode | Never rely on Basic Auth alone |
| **No logout** | Credentials stay in browser | Use other methods for web apps |
| **Password storage** | Must be hashed | Use secure password hashing |

---

## Token-Based Authentication

### What are Tokens?

**Token**: A piece of data that acts like a digital key. Once you get a token, you use it for subsequent requests instead of sending your password every time.

**📌 Think of it like**: A wristband at a theme park - you show your ticket once, get the wristband, then just show the wristband for rides.

### Token Authentication Flow

```mermaid
sequenceDiagram
    participant Client as Client App
    participant Server as API Server
    participant Auth as Auth Service

    Client->>Auth: 1. Login with username/password
    Auth->>Client: 2. Return access token
    Client->>Server: 3. Request with token in header
    Server->>Client: 4. Validate token, return data
```

### Common Token Types

| Type | Description | Use Case |
|------|-------------|----------|
| **Opaque Tokens** | Random strings stored on server | Simple implementations |
| **JWT (JSON Web Tokens)** | Self-contained tokens with claims | Modern web/mobile apps |
| **Reference Tokens** | Token ID stored, data looked up | Security-focused apps |

### JWT (JSON Web Tokens) Explained

**JWT**: A self-contained token with three parts separated by dots:
`header.payload.signature`

**Structure:**
```javascript
// Header (algorithm and token type)
{
  "alg": "HS256",
  "typ": "JWT"
}

// Payload (data/claims)
{
  "sub": "1234567890",
  "name": "John Doe",
  "exp": 1640995200
}

// Signature (verification)
HMACSHA256(
  base64UrlEncode(header) + "." +
  base64UrlEncode(payload),
  your-256-bit-secret
)
```

### JWT Implementation Example

**Login Request:**
```http
POST /api/auth/login HTTP/1.1
Content-Type: application/json

{
  "email": "user@example.com",
  "password": "userpassword123"
}
```

**Token Response:**
```json
{
  "accessToken": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...",
  "refreshToken": "def5020012345678...",
  "tokenType": "Bearer",
  "expiresIn": 3600
}
```

**API Request with Token:**
```http
GET /api/users/profile HTTP/1.1
Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...
```

### Token Storage Best Practices

| Location | Security | Pros | Cons |
|----------|----------|------|------|
| **HTTP-Only Cookies** | High | Protected from XSS | CSRF protection needed |
| **LocalStorage** | Low | Easy to access | Vulnerable to XSS |
| **SessionStorage** | Low | Per-tab storage | Vulnerable to XSS |
| **Memory** | High | Most secure | Lost on refresh |

---

## Session-Based Authentication

### What are Sessions?

**Session-Based Auth**: Server creates a session for each user after login and gives the client a session ID (usually in a cookie).

**📌 Think of it like**: Getting a locker key at the gym. You show your ID once, get a key, and use that key to access your locker.

### Session Authentication Flow

```mermaid
sequenceDiagram
    participant Client as Browser
    participant Server as Web Server
    participant Database as Session Store

    Client->>Server: 1. Login with credentials
    Server->>Database: 2. Create session
    Database->>Server: 3. Return session ID
    Server->>Client: 4. Set session cookie
    Client->>Server: 5. Request with session cookie
    Server->>Database: 6. Validate session
    Database->>Server: 7. Return session data
    Server->>Client: 8. Return protected data
```

### Session Storage Options

| Method | Scalability | Performance | Security | Use Case |
|--------|-------------|-------------|----------|----------|
| **Memory** | Poor | Fastest | Low | Development |
| **Database** | Good | Slow | Medium | Small apps |
| **Redis** | Excellent | Fast | High | Production |
| **File System** | Poor | Medium | Medium | Simple setups |

### Session Configuration Example

```javascript
// Express.js session configuration
const session = require('express-session');

app.use(session({
  secret: 'your-secret-key',
  resave: false,
  saveUninitialized: false,
  cookie: {
    secure: process.env.NODE_ENV === 'production', // HTTPS only in production
    httpOnly: true,                               // Prevent XSS
    maxAge: 24 * 60 * 60 * 1000                   // 24 hours
  },
  store: redisStore // Use Redis for production
}));
```

### Sessions vs Tokens

| Feature | Sessions | Tokens |
|---------|----------|--------|
| **State Management** | Server-side | Stateless |
| **Scalability** | Requires session store | Naturally scalable |
| **Mobile Support** | Limited | Excellent |
| **Logout** | Server-controlled | Client-controlled |
| **Storage** | Server | Client (JWT) |

---

## OAuth 2.0

### What is OAuth 2.0?

**OAuth 2.0**: An authorization framework that allows applications to obtain limited access to user accounts on an HTTP service.

**📌 Think of it like**: A valet key for your car - you give it to the valet so they can park your car, but they can't open your trunk or drive away.

### OAuth 2.0 Roles

| Role | Description | Example |
|------|-------------|---------|
| **Resource Owner** | User who owns the data | You, the user |
| **Client** | Application requesting access | Photo editing app |
| **Resource Server** | API that hosts the data | Google Photos API |
| **Authorization Server** | Server that issues tokens | Google's OAuth server |

### OAuth 2.0 Grant Types

| Grant Type | Description | Use Case |
|------------|-------------|----------|
| **Authorization Code** | Code exchanged for token | Web applications |
| **Implicit** | Token returned directly | Single-page apps |
| **Client Credentials** | App-to-app authentication | Service accounts |
| **Resource Owner Password** | Username/password exchanged | Trusted apps |
| **Device Code** | Code displayed on device | TV/limited input devices |

### Authorization Code Flow (Most Common)

```mermaid
sequenceDiagram
    participant User as Resource Owner
    participant Client as App
    participant Auth as Authorization Server
    participant API as Resource Server

    User->>Client: 1. Click "Login with Google"
    Client->>Auth: 2. Redirect to auth URL
    User->>Auth: 3. Login and grant permission
    Auth->>Client: 4. Redirect with auth code
    Client->>Auth: 5. Exchange code for token
    Auth->>Client: 6. Return access token
    Client->>API: 7. Request with access token
    API->>Client: 8. Return protected data
```

### OAuth 2.0 Implementation Steps

**Step 1: Register Application**
- Get client ID and client secret
- Set redirect URIs
- Choose grant types

**Step 2: Redirect User for Authorization**
```javascript
const authUrl = `https://accounts.google.com/o/oauth2/v2/auth?` +
  `client_id=${CLIENT_ID}&` +
  `redirect_uri=${REDIRECT_URI}&` +
  `scope=profile email&` +
  `response_type=code`;

res.redirect(authUrl);
```

**Step 3: Exchange Code for Token**
```javascript
// Backend receives code from redirect
const response = await fetch('https://oauth2.googleapis.com/token', {
  method: 'POST',
  headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
  body: new URLSearchParams({
    code: authCode,
    client_id: CLIENT_ID,
    client_secret: CLIENT_SECRET,
    redirect_uri: REDIRECT_URI,
    grant_type: 'authorization_code'
  })
});
```

### Common OAuth 2.0 Providers

| Provider | Documentation | Popular Scopes |
|----------|----------------|----------------|
| **Google** | developers.google.com | profile, email, drive |
| **GitHub** | docs.github.com/en/developers | user, repo, read:user |
| **Facebook** | developers.facebook.com | email, public_profile |
| **Microsoft** | docs.microsoft.com/azure/active-directory | openid, profile, email |

---

## Multi-Factor Authentication (MFA)

### What is MFA?

**MFA (Multi-Factor Authentication)**: Security system requiring more than one method of authentication from independent categories.

**📌 Think of it like**: Using both your key and fingerprint to open a high-security door.

### Authentication Factors

| Factor | Examples | Security Level |
|--------|----------|----------------|
| **Something you know** | Password, PIN, security questions | Basic |
| **Something you have** | Phone, hardware token, smart card | Strong |
| **Something you are** | Fingerprint, face, voice, iris | Very Strong |
| **Somewhere you are** | GPS location, IP address | Medium |

### Common MFA Methods

| Method | Description | Pros | Cons |
|--------|-------------|------|------|
| **SMS/Email Codes** | Code sent to phone/email | Simple, no extra app | Less secure, can be intercepted |
| **Authenticator Apps** | TOTP codes from apps | Very secure, offline | Requires smartphone |
| **Hardware Keys** | USB security keys | Extremely secure | Cost, device required |
| **Biometrics** | Fingerprint, face scan | Convenient | Privacy concerns |

### TOTP Implementation (Time-based One-Time Password)

**Algorithm Flow:**
1. Server generates secret key for user
2. User adds secret to authenticator app
3. Both generate 6-digit codes every 30 seconds
4. User enters code to verify

**Code Example:**
```javascript
// Generate TOTP secret
const speakeasy = require('speakeasy');
const secret = speakeasy.generateSecret({
  name: 'MyApp (user@example.com)',
  issuer: 'MyApp'
});

// Verify TOTP token
const verified = speakeasy.totp.verify({
  secret: userSecret,
  encoding: 'base32',
  token: userInputCode,
  window: 1 // Allow 30 seconds time drift
});
```

### MFA Implementation Strategy

```javascript
// Login flow with MFA
async function loginWithMFA(req, res) {
  const { username, password, mfaCode } = req.body;

  // Step 1: Validate username/password
  const user = await validateCredentials(username, password);
  if (!user) {
    return res.status(401).json({ error: 'Invalid credentials' });
  }

  // Step 2: Check MFA requirement
  if (user.mfaEnabled) {
    if (!mfaCode) {
      return res.status(401).json({
        error: 'MFA code required',
        requiresMFA: true
      });
    }

    // Step 3: Verify MFA code
    const isValidMFA = await verifyMFACode(user.mfaSecret, mfaCode);
    if (!isValidMFA) {
      return res.status(401).json({ error: 'Invalid MFA code' });
    }
  }

  // Step 4: Generate access token
  const token = generateJWT(user);
  res.json({ accessToken: token });
}
```

---

## API Keys

### What are API Keys?

**API Key**: A unique identifier used to authenticate a user, developer, or calling program to an API.

**📌 Think of it like**: A library card number that identifies who you are when checking out books.

### API Key Types

| Type | Description | Use Case |
|------|-------------|----------|
| **Public Key** | Identifies client, no auth | Public APIs, rate limiting |
| **Private Key** | Secret authentication | Server-to-server calls |
| **Limited Scope** | Permissions-based access | Third-party integrations |

### API Key Implementation

**Key Generation:**
```javascript
const crypto = require('crypto');

function generateAPIKey() {
  return {
    keyId: `ak_${crypto.randomBytes(16).toString('hex')}`,
    keySecret: crypto.randomBytes(32).toString('hex')
  };
}

// Result:
// {
//   keyId: "ak_1a2b3c4d5e6f7g8h9i0j",
//   keySecret: "1a2b3c4d5e6f7g8h9i0j1k2l3m4n5o6p7q8r9s0t"
// }
```

**API Key Usage:**
```http
GET /api/users HTTP/1.1
X-API-Key: ak_1a2b3c4d5e6f7g8h9i0j
X-API-Secret: 1a2b3c4d5e6f7g8h9i0j1k2l3m4n5o6p7q8r9s0t
```

### API Key Security Best Practices

| Practice | Description | Implementation |
|----------|-------------|----------------|
| **HTTPS Only** | Never transmit over HTTP | Enforce HTTPS in server |
| **Key Rotation** | Change keys regularly | Implement key expiration |
| **Rate Limiting** | Prevent abuse | Per-key rate limits |
| **Scoping** | Limit key permissions | Define key capabilities |
| **Auditing** | Log all usage | Track key access patterns |

---

## Authentication Best Practices

### Security Checklist

| Practice | Why Important | Implementation |
|----------|----------------|----------------|
| **HTTPS Only** | Prevents man-in-the-middle attacks | Use SSL/TLS certificates |
| **Password Hashing** | Protects passwords in database | bcrypt, Argon2, scrypt |
| **Rate Limiting** | Prevents brute force attacks | Login attempt limits |
| **Input Validation** | Prevents injection attacks | Sanitize all inputs |
| **Token Expiration** | Limits window of abuse | Short-lived access tokens |
| **Secure Storage** | Protects secrets | Environment variables, vaults |

### Password Security

**Hashing Algorithms (Recommended):**
- **bcrypt**: Adaptive, good for passwords
- **Argon2**: Winner of password hashing competition
- **scrypt**: Memory-intensive, resistant to GPU attacks

```javascript
// bcrypt example
const bcrypt = require('bcrypt');

async function hashPassword(password) {
  const saltRounds = 12;
  return await bcrypt.hash(password, saltRounds);
}

async function verifyPassword(password, hash) {
  return await bcrypt.compare(password, hash);
}
```

### Token Security

| Security Measure | Implementation |
|------------------|----------------|
| **Short-lived tokens** | 15-60 minute expiration |
| **Refresh tokens** | Long-lived but securely stored |
| **Secure storage** | HttpOnly, Secure cookies |
| **Token rotation** | New tokens on each use |
| **Audience/Scope** | Limit token usage context |

### Monitoring and Logging

**Key Authentication Events to Log:**
- Successful logins
- Failed login attempts
- Token generation/refresh
- Password changes
- MFA challenges
- API key usage

**Red Flags to Monitor:**
- Multiple failed attempts
- Unusual locations/times
- Rapid successive requests
- Token abuse patterns
- API key anomalies

---

## Choosing the Right Authentication Method

### Decision Framework

```mermaid
graph TD
    A[What's Your Use Case?] --> B{Public or Internal?}

    B -->|Public| C{Web or Mobile App?}
    B -->|Internal| D{Simple or Complex?}

    C -->|Web| E[OAuth 2.0 + JWT]
    C -->|Mobile| F[OAuth 2.0 + Refresh Tokens]

    D -->|Simple| G[API Keys]
    D -->|Complex| H[JWT + Sessions]

    E --> I{High Security Needed?}
    F --> I
    H --> I

    I -->|Yes| J[Add MFA]
    I -->|No| K[Standard Auth]
```

### Comparison Matrix

| Method | Complexity | Security | Scalability | Mobile Support | Best For |
|--------|------------|----------|-------------|----------------|----------|
| **Basic Auth** | Very Low | Low | High | Good | Internal APIs |
| **Session-Based** | Medium | Medium | Medium | Poor | Traditional Web Apps |
| **JWT Tokens** | Medium | High | High | Excellent | Modern Web/Mobile |
| **OAuth 2.0** | High | High | High | Excellent | Third-party Integration |
| **API Keys** | Low | Medium | High | Excellent | Service-to-Service |

### Implementation Recommendations

| Scenario | Recommended Method | Additional Security |
|----------|-------------------|---------------------|
| **Modern SPA** | JWT + Refresh Tokens | CSRF protection |
| **Mobile App** | OAuth 2.0 + JWT | Biometric auth |
| **Microservices** | API Keys + mTLS | Zero-trust network |
| **Public API** | API Keys + OAuth 2.0 | Rate limiting |
| **Enterprise** | SAML + MFA | SSO integration |

---

## Interview Questions

### Basic Questions

1. **What's the difference between authentication and authorization?**
   - Authentication: Who you are (proving identity)
   - Authorization: What you can do (permissions)

2. **How does JWT work?**
   - Self-contained token with header, payload, signature
   - Signed with secret key to prevent tampering
   - Contains claims about the user

3. **Why is Basic Authentication insecure?**
   - Sends credentials in Base64 (easily decoded)
   - Should only be used with HTTPS
   - No built-in logout mechanism

### Intermediate Questions

4. **What's the difference between sessions and tokens?**
   - Sessions: Server-side state, cookies
   - Tokens: Client-side, stateless, JWT
   - Sessions scale poorly, tokens scale naturally

5. **How does OAuth 2.0 work?**
   - Authorization framework for delegated access
   - Uses authorization codes, access tokens, refresh tokens
   - Common grant types: Authorization Code, Client Credentials

6. **What is MFA and why is it important?**
   - Multi-Factor Authentication
   - Uses multiple authentication factors
   - Protects against password compromise

### Advanced Questions

7. **How would you prevent JWT token theft?**
   - Use short expiration times
   - Store in secure HttpOnly cookies
   - Implement token rotation
   - Use refresh tokens properly

8. **What are the security risks of API keys?**
   - Can be stolen from code repositories
   - No expiration by default
   - Hard to revoke quickly
   - Need proper scoping and rate limiting

9. **How would you implement secure password storage?**
   - Use modern hashing algorithms (bcrypt, Argon2)
   - Never store plain passwords
   - Use salt to prevent rainbow table attacks
   - Implement password policies

---

## Summary

### Key Takeaways

1. **Basic Auth**: Simple but limited, use only with HTTPS
2. **JWT Tokens**: Stateless, scalable, great for modern apps
3. **Sessions**: Traditional, server-side, good for web apps
4. **OAuth 2.0**: Industry standard for third-party access
5. **MFA**: Essential for high-security applications
6. **API Keys**: Perfect for service-to-service communication

### Security Best Practices Checklist

- [ ] Always use HTTPS for authentication
- [ ] Hash passwords with modern algorithms
- [ ] Use short-lived tokens with refresh mechanism
- [ ] Implement rate limiting for login attempts
- [ ] Add MFA for sensitive operations
- [ ] Log authentication events
- [ ] Store secrets securely (environment variables)
- [ ] Regularly rotate keys and secrets

### Implementation Decision Flow

```mermaid
graph LR
    A[Start] --> B{Public API?}
    B -->|Yes| C[OAuth 2.0 + API Keys]
    B -->|No| D{Web App?}
    D -->|Yes| E[Sessions + CSRF Protection]
    D -->|No| F[JWT + Refresh Tokens]

    C --> G{High Security?}
    E --> G
    F --> G

    G -->|Yes| H[Add MFA]
    G -->|No| I[Standard Implementation]
```

**Next Up**: Chapter 05 explores Authorization Methods, covering how to control what authenticated users can access and do.