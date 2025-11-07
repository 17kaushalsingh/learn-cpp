# Chapter 12: Standards and Compliance

## Introduction to API Compliance

API compliance involves ensuring that your APIs adhere to various regulatory standards, industry guidelines, and data protection requirements. Non-compliance can result in legal penalties, loss of customer trust, and damage to your organization's reputation.

### Why Compliance Matters for APIs

- **Legal Requirements**: Many regulations mandate specific data handling practices
- **Data Privacy**: Protect user personal information and sensitive data
- **Security Standards**: Implement proper security controls and monitoring
- **International Operations**: Comply with regulations across different jurisdictions
- **Customer Trust**: Build confidence with proper data protection practices

### Key Compliance Areas

1. **Data Privacy**: GDPR, CCPA, PII protection
2. **Payment Security**: PCI DSS for financial APIs
3. **Healthcare**: HIPAA for medical data APIs
4. **Industry Standards**: ISO standards, SOC compliance
5. **Regional Regulations**: Country-specific data protection laws

## GDPR (General Data Protection Regulation)

### What is GDPR?

GDPR is a comprehensive data protection regulation that applies to all organizations processing personal data of individuals in the European Union, regardless of where the organization is located.

### Key GDPR Requirements for APIs

```javascript
// GDPR Compliance Framework
class GDPRCompliance {
  constructor() {
    this.dataProcessingRegistry = new Map();
    this.consentManager = new ConsentManager();
    this.dataProtectionOfficer = null;
  }

  // Register data processing activities
  registerDataProcessing(activity) {
    const requiredFields = {
      name: 'Processing activity name',
      purpose: 'Business purpose',
      categories: 'Data categories processed',
      recipients: 'Data recipients',
      retention: 'Data retention period',
      legalBasis: 'Legal basis for processing'
    };

    // Validate all required fields
    Object.keys(requiredFields).forEach(field => {
      if (!activity[field]) {
        throw new Error(`Missing required field: ${field} (${requiredFields[field]})`);
      }
    });

    this.dataProcessingRegistry.set(activity.id, {
      ...activity,
      createdAt: new Date(),
      lastReviewed: new Date()
    });

    console.log(`Registered data processing: ${activity.name}`);
  }

  // Handle data subject requests
  async handleDataSubjectRequest(request) {
    const { type, subjectId, identityProof } = request;

    // Verify identity before processing request
    const isValidIdentity = await this.verifyIdentity(subjectId, identityProof);
    if (!isValidIdentity) {
      throw new Error('Identity verification failed');
    }

    switch (type) {
      case 'access':
        return await this.handleAccessRequest(subjectId);
      case 'rectification':
        return await this.handleRectificationRequest(subjectId, request.corrections);
      case 'erasure':
        return await this.handleErasureRequest(subjectId, request.reason);
      case 'portability':
        return await this.handlePortabilityRequest(subjectId);
      case 'restriction':
        return await this.handleRestrictionRequest(subjectId, request.scope);
      case 'objection':
        return await this.handleObjectionRequest(subjectId, request.reason);
      default:
        throw new Error('Invalid request type');
    }
  }

  // Handle right to access
  async handleAccessRequest(subjectId) {
    const userData = await this.collectAllPersonalData(subjectId);

    const response = {
      requestId: generateRequestId(),
      subjectId,
      dataProvided: userData,
      processingActivities: this.getSubjectProcessingActivities(subjectId),
      recipients: this.getDataRecipients(subjectId),
      retentionPeriods: this.getRetentionPeriods(subjectId),
      generatedAt: new Date().toISOString()
    };

    // Log the access request
    this.logDataSubjectRequest({
      type: 'access',
      subjectId,
      processedAt: new Date(),
      dataProvided: Object.keys(userData)
    });

    return response;
  }

  // Handle right to erasure (Right to be forgotten)
  async handleErasureRequest(subjectId, reason) {
    const processingActivities = this.getSubjectProcessingActivities(subjectId);
    const erasureResults = [];

    for (const activity of processingActivities) {
      try {
        // Check if erasure is required or exceptions apply
        const mustErase = this.checkErasureRequirement(activity, reason);

        if (mustErase) {
          const result = await this.erasePersonalData(subjectId, activity);
          erasureResults.push({
            activityId: activity.id,
            success: true,
            recordsDeleted: result.count
          });
        } else {
          erasureResults.push({
            activityId: activity.id,
            success: false,
            reason: 'Legal basis for retention exists'
          });
        }
      } catch (error) {
        erasureResults.push({
          activityId: activity.id,
          success: false,
          error: error.message
        });
      }
    }

    return {
      requestId: generateRequestId(),
      subjectId,
      erasureResults,
      processedAt: new Date().toISOString()
    };
  }

  // Data breach notification
  async reportDataBreach(breach) {
    const breachReport = {
      id: generateBreachId(),
      discoveredAt: new Date(),
      description: breach.description,
      dataCategories: breach.dataCategories,
      affectedSubjects: breach.affectedSubjects,
      consequences: breach.potentialConsequences,
      measures: breach.mitigationMeasures,
      dpoNotified: false,
      supervisoryAuthorityNotified: false,
      dataSubjectsNotified: false
    };

    // Assess if breach requires notification
    const requiresNotification = this.assessBreachNotificationRequirement(breach);

    if (requiresNotification) {
      // Notify Data Protection Officer within 72 hours
      await this.notifyDPO(breachReport);
      breachReport.dpoNotified = true;

      // Determine if supervisory authority needs to be notified
      if (breach.riskToRights === 'high') {
        await this.notifySupervisoryAuthority(breachReport);
        breachReport.supervisoryAuthorityNotified = true;
      }

      // Notify data subjects if high risk to rights and freedoms
      if (breach.riskToRights === 'high') {
        await this.notifyDataSubjects(breachReport);
        breachReport.dataSubjectsNotified = true;
      }
    }

    return breachReport;
  }
}

// GDPR-compliant API middleware
const gdprMiddleware = {
  // Data minimization middleware
  dataMinimization: (allowedFields) => {
    return (req, res, next) => {
      if (req.method === 'GET') {
        // Only return allowed fields in response
        const originalJson = res.json;
        res.json = function(data) {
          if (Array.isArray(data)) {
            const minimizedData = data.map(item => {
              const result = {};
              allowedFields.forEach(field => {
                if (item[field] !== undefined) {
                  result[field] = item[field];
                }
              });
              return result;
            });
            return originalJson.call(this, minimizedData);
          } else {
            const result = {};
            allowedFields.forEach(field => {
              if (data[field] !== undefined) {
                result[field] = data[field];
              }
            });
            return originalJson.call(this, result);
          }
        };
      }
      next();
    };
  },

  // Consent verification middleware
  consentRequired: (consentType) => {
    return async (req, res, next) => {
      const userId = req.user?.id;
      if (!userId) {
        return res.status(401).json({ error: 'Authentication required' });
      }

      const hasConsent = await consentManager.hasConsent(userId, consentType);
      if (!hasConsent) {
        return res.status(403).json({
          error: 'Consent required',
          consentType,
          consentUrl: `/api/consent/${consentType}`
        });
      }

      next();
    };
  },

  // Data retention policy middleware
  dataRetention: (retentionPeriod) => {
    return async (req, res, next) => {
      // Add retention metadata to responses
      res.setHeader('X-Data-Retention', retentionPeriod);
      res.setHeader('X-Retention-Expires', new Date(Date.now() + parseRetentionPeriod(retentionPeriod)).toISOString());

      next();
    };
  }
};

// Implementation examples
app.get('/api/users',
  authenticateUser,
  gdprMiddleware.dataMinimization(['id', 'name', 'email', 'createdAt']),
  gdprMiddleware.consentRequired('profile_access'),
  gdprMiddleware.dataRetention('2 years'),
  async (req, res) => {
    const users = await User.find();
    res.json(users);
  }
);

// Data subject request endpoint
app.post('/api/privacy/requests', authenticateUser, async (req, res) => {
  try {
    const request = {
      type: req.body.type,
      subjectId: req.user.id,
      identityProof: req.body.identityProof
    };

    const result = await gdprCompliance.handleDataSubjectRequest(request);
    res.json(result);
  } catch (error) {
    res.status(400).json({ error: error.message });
  }
});
```

### GDPR Data Categories and Processing

```javascript
// GDPR Data Classification
class GDPRDataClassifier {
  constructor() {
    this.personalDataCategories = {
      // Special Category Data (Requires explicit consent)
      specialCategories: [
        'racialOrEthnicOrigin',
        'politicalOpinions',
        'religiousBeliefs',
        'tradeUnionMembership',
        'geneticData',
        'biometricData',
        'healthData',
        'sexualOrientation'
      ],

      // Standard Personal Data
      personalIdentifiers: [
        'name',
        'email',
        'phone',
        'address',
        'nationalId',
        'passportNumber'
      ],

      // Online Identifiers
      onlineIdentifiers: [
        'ipAddress',
        'cookieId',
        'deviceFingerprint',
        'advertisingId'
      ],

      // Location Data
      locationData: [
        'gpsCoordinates',
        'geolocation',
        'cellTowerData'
      ]
    };
  }

  classifyData(dataObject) {
    const classification = {
      isPersonalData: false,
      categories: [],
      specialCategories: [],
      processingLegalBasis: [],
      protectionLevel: 'standard'
    };

    // Check each field against categories
    Object.keys(dataObject).forEach(field => {
      const value = dataObject[field];

      if (this.isPersonalIdentifier(field, value)) {
        classification.isPersonalData = true;
        classification.categories.push('personalIdentifiers');
      }

      if (this.isSpecialCategoryData(field, value)) {
        classification.isPersonalData = true;
        classification.specialCategories.push(field);
        classification.protectionLevel = 'special';
      }

      if (this.isOnlineIdentifier(field, value)) {
        classification.isPersonalData = true;
        classification.categories.push('onlineIdentifiers');
      }

      if (this.isLocationData(field, value)) {
        classification.isPersonalData = true;
        classification.categories.push('locationData');
      }
    });

    // Determine legal basis based on data categories
    classification.processingLegalBasis = this.determineLegalBasis(classification);

    return classification;
  }

  determineLegalBasis(classification) {
    const legalBasis = [];

    if (classification.specialCategories.length > 0) {
      legalBasis.push('explicit_consent');
    }

    if (classification.categories.includes('personalIdentifiers')) {
      legalBasis.push('contractual_necessity');
    }

    if (classification.categories.includes('locationData')) {
      legalBasis.push('legitimate_interest');
    }

    if (classification.categories.includes('onlineIdentifiers')) {
      legalBasis.push('consent');
    }

    return legalBasis.length > 0 ? legalBasis : ['consent'];
  }
}
```

## CCPA (California Consumer Privacy Act)

### What is CCPA?

CCPA is a privacy law that grants California residents specific rights regarding their personal information, including the right to know, delete, and opt-out of the sale of their personal information.

### CCPA Implementation for APIs

```javascript
// CCPA Compliance Framework
class CCPACompliance {
  constructor() {
    this.consumerRequests = new Map();
    this.optOutDatabase = new OptOutDatabase();
    this.dataSaleRegistry = new DataSaleRegistry();
  }

  // Handle CCPA consumer requests
  async handleConsumerRequest(request) {
    const { type, consumerId, verificationInfo } = request;

    // Verify consumer identity
    const isValidConsumer = await this.verifyConsumerIdentity(consumerId, verificationInfo);
    if (!isValidConsumer) {
      throw new Error('Consumer identity verification failed');
    }

    const requestId = this.generateRequestId();
    const consumerRequest = {
      id: requestId,
      type,
      consumerId,
      status: 'processing',
      createdAt: new Date(),
      dueDate: new Date(Date.now() + 45 * 24 * 60 * 60 * 1000), // 45 days
      responseSent: false
    };

    this.consumerRequests.set(requestId, consumerRequest);

    try {
      let response;
      switch (type) {
        case 'know':
          response = await this.handleKnowRequest(consumerId);
          break;
        case 'delete':
          response = await this.handleDeleteRequest(consumerId);
          break;
        case 'opt-out':
          response = await this.handleOptOutRequest(consumerId);
          break;
        case 'opt-in':
          response = await this.handleOptInRequest(consumerId);
          break;
        default:
          throw new Error('Invalid request type');
      }

      consumerRequest.response = response;
      consumerRequest.status = 'completed';
      consumerRequest.responseSent = true;

      return { requestId, ...response };

    } catch (error) {
      consumerRequest.status = 'failed';
      consumerRequest.error = error.message;
      throw error;
    }
  }

  // Right to Know - Information disclosure
  async handleKnowRequest(consumerId) {
    const personalInfo = await this.collectPersonalInformation(consumerId);
    const businessPurposes = await this.getBusinessPurposes();
    const dataSources = await this.getDataSources(consumerId);
    const thirdPartyDisclosures = await this.getThirdPartyDisclosures(consumerId);

    return {
      personalInfoCollected: personalInfo,
      businessPurposes: businessPurposes,
      dataSources: dataSources,
      thirdPartyDisclosures: thirdPartyDisclosures,
      infoSoldOrDisclosed: await this.getInfoSoldOrDisclosed(consumerId),
      financialIncentives: await this.getFinancialIncentives(),
      generatedAt: new Date().toISOString()
    };
  }

  // Right to Delete
  async handleDeleteRequest(consumerId) {
    const deletionResults = [];
    const dataCategories = await this.getAllDataCategories(consumerId);

    for (const category of dataCategories) {
      try {
        const result = await this.deleteConsumerData(consumerId, category);
        deletionResults.push({
          category: category.name,
          recordsDeleted: result.count,
          success: true
        });
      } catch (error) {
        deletionResults.push({
          category: category.name,
          success: false,
          error: error.message
        });
      }
    }

    // Notify third parties of deletion request
    await this.notifyThirdPartiesOfDeletion(consumerId);

    return {
      deletionResults,
      categoriesProcessed: deletionResults.length,
      successfulDeletions: deletionResults.filter(r => r.success).length,
      processedAt: new Date().toISOString()
    };
  }

  // Right to Opt Out of Sale
  async handleOptOutRequest(consumerId) {
    // Add consumer to opt-out list
    const optOutRecord = {
      consumerId,
      optOutDate: new Date(),
      optOutMethod: 'api_request',
      ipAddress: this.getCurrentRequestIp(),
      userAgent: this.getCurrentUserAgent()
    };

    await this.optOutDatabase.addOptOut(optOutRecord);

    // Update existing data records to mark as non-saleable
    await this.markDataAsNonSaleable(consumerId);

    // Generate consumer-specific opt-out signal
    const optOutSignal = this.generateOptOutSignal(consumerId);

    // Send opt-out signal to known third parties
    await this.broadcastOptOutSignal(optOutSignal);

    return {
      optOutConfirmation: true,
      optOutSignal: optOutSignal,
      effectiveDate: optOutRecord.optOutDate,
      processedAt: new Date().toISOString()
    };
  }

  // Check if consumer has opted out
  async hasOptedOut(consumerId) {
    return await this.optOutDatabase.hasOptOut(consumerId);
  }

  // Generate opt-out signal for third-party transmission
  generateOptOutSignal(consumerId) {
    return {
      version: 1,
      opt_out_sale: true,
      opt_out_sale_sharing: true,
      consent_language: 'EN',
      gpc_signal: false, // Global Privacy Control signal
      ccpa_applies: true,
      usp_string: `1YYY---`, // Opt-out of sale encoded string
      consumer_id: consumerId,
      generated_at: new Date().toISOString(),
      expires_at: new Date(Date.now() + 365 * 24 * 60 * 60 * 1000).toISOString() // 1 year
    };
  }
}

// CCPA API Middleware
const ccpaMiddleware = {
  // Opt-out verification middleware
  checkOptOut: async (req, res, next) => {
    const consumerId = req.user?.id || req.cookies.consumer_id;

    if (consumerId) {
      const hasOptedOut = await ccpaCompliance.hasOptedOut(consumerId);
      req.ccpaOptedOut = hasOptedOut;

      if (hasOptedOut && req.path.includes('/sell') || req.path.includes('/share')) {
        return res.status(403).json({
          error: 'Consumer has opted out of data sale/sharing',
          optOutDate: await ccpaCompliance.getOptOutDate(consumerId)
        });
      }
    }

    next();
  },

  // Data sale tracking middleware
  trackDataSale: (saleType) => {
    return async (req, res, next) => {
      const originalJson = res.json;

      res.json = function(data) {
        // Track data sale if not opted out
        if (!req.ccpaOptedOut && req.user?.id) {
          ccpaCompliance.recordDataSale({
            consumerId: req.user.id,
            saleType: saleType,
            dataCategories: Object.keys(data),
            recipient: req.get('X-Client-ID') || 'unknown',
            timestamp: new Date()
          });
        }
        return originalJson.call(this, data);
      };

      next();
    };
  },

  // Global Privacy Control signal handler
  checkGPCSignal: (req, res, next) => {
    const gpcSignal = req.get('Sec-GPC') === '1' || req.get('GPC') === '1';

    if (gpcSignal) {
      // Treat as opt-out signal
      req.ccpaOptedOut = true;
      req.gpcSignal = true;
    }

    next();
  }
};

// CCPA endpoints
app.post('/api/privacy/ccpa/requests', authenticateUser, async (req, res) => {
  try {
    const request = {
      type: req.body.type,
      consumerId: req.user.id,
      verificationInfo: req.body.verification
    };

    const result = await ccpaCompliance.handleConsumerRequest(request);
    res.json(result);
  } catch (error) {
    res.status(400).json({ error: error.message });
  }
});

// Opt-out endpoint
app.post('/api/privacy/ccpa/opt-out', ccpaMiddleware.checkGPCSignal, async (req, res) => {
  try {
    const consumerId = req.user?.id || req.body.consumer_id;

    const result = await ccpaCompliance.handleOptOutRequest(consumerId);

    // Set opt-out cookie
    res.cookie('ccpa_opt_out', 'true', {
      maxAge: 365 * 24 * 60 * 60 * 1000, // 1 year
      httpOnly: true,
      secure: process.env.NODE_ENV === 'production'
    });

    res.json(result);
  } catch (error) {
    res.status(400).json({ error: error.message });
  }
});
```

## PCI DSS (Payment Card Industry Data Security Standard)

### What is PCI DSS?

PCI DSS is a set of security standards designed to ensure that all companies that accept, process, store, or transmit credit card information maintain a secure environment.

### PCI DSS Implementation for Payment APIs

```javascript
// PCI DSS Compliance Framework
class PCIDSSCompliance {
  constructor() {
    this.encryptionKeyManager = new EncryptionKeyManager();
    this.tokenizationService = new TokenizationService();
    this.auditLogger = new AuditLogger();
    this.networkSegmentation = new NetworkSegmentation();
  }

  // Secure credit card processing
  async processPayment(paymentData) {
    const transactionId = this.generateTransactionId();

    try {
      // Step 1: Validate input data
      this.validatePaymentData(paymentData);

      // Step 2: Tokenize sensitive card data
      const tokenizedData = await this.tokenizeCardData(paymentData);

      // Step 3: Encrypt sensitive information
      const encryptedData = await this.encryptSensitiveData(tokenizedData);

      // Step 4: Process payment through payment gateway
      const result = await this.sendToPaymentGateway(encryptedData);

      // Step 5: Store transaction record without PAN
      await this.storeTransactionRecord({
        transactionId,
        cardToken: tokenizedData.cardToken,
        amount: paymentData.amount,
        currency: paymentData.currency,
        merchantId: paymentData.merchantId,
        status: result.status,
        responseCode: result.responseCode,
        timestamp: new Date(),
        // Store only last 4 digits for display
        cardLast4: paymentData.cardNumber.slice(-4)
      });

      this.auditLogger.logPaymentTransaction({
        transactionId,
        merchantId: paymentData.merchantId,
        amount: paymentData.amount,
        currency: paymentData.currency,
        status: result.status,
        timestamp: new Date()
      });

      return {
        transactionId,
        status: result.status,
        authCode: result.authCode,
        cardLast4: paymentData.cardNumber.slice(-4),
        timestamp: new Date()
      };

    } catch (error) {
      this.auditLogger.logPaymentError({
        transactionId,
        error: error.message,
        timestamp: new Date()
      });
      throw error;
    }
  }

  // Card data tokenization
  async tokenizeCardData(paymentData) {
    const cardData = {
      cardNumber: paymentData.cardNumber,
      expiryMonth: paymentData.expiryMonth,
      expiryYear: paymentData.expiryYear,
      cvv: paymentData.cvv
    };

    // Validate card format
    this.validateCardFormat(cardData);

    // Generate token
    const token = await this.tokenizationService.tokenize(cardData);

    return {
      cardToken: token,
      cardLast4: cardData.cardNumber.slice(-4),
      cardBrand: this.getCardBrand(cardData.cardNumber),
      expiryMonth: cardData.expiryMonth,
      expiryYear: cardData.expiryYear
      // Note: CVV is never stored, only used for one-time processing
    };
  }

  // Payment card validation
  validateCardFormat(cardData) {
    // Luhn algorithm check
    if (!this.luhnCheck(cardData.cardNumber)) {
      throw new Error('Invalid card number');
    }

    // Check card number length
    if (cardData.cardNumber.length < 13 || cardData.cardNumber.length > 19) {
      throw new Error('Invalid card number length');
    }

    // Check expiry date
    const currentDate = new Date();
    const expiryDate = new Date(
      parseInt(cardData.expiryYear),
      parseInt(cardData.expiryMonth) - 1,
      1
    );

    if (expiryDate <= currentDate) {
      throw new Error('Card has expired');
    }

    // CVV validation
    const cvvLength = cardData.cvv.length;
    if (cvvLength !== 3 && cvvLength !== 4) {
      throw new Error('Invalid CVV');
    }
  }

  // Luhn algorithm for card number validation
  luhnCheck(cardNumber) {
    const digits = cardNumber.replace(/\D/g, '');
    let sum = 0;
    let isEven = false;

    for (let i = digits.length - 1; i >= 0; i--) {
      let digit = parseInt(digits[i]);

      if (isEven) {
        digit *= 2;
        if (digit > 9) {
          digit -= 9;
        }
      }

      sum += digit;
      isEven = !isEven;
    }

    return sum % 10 === 0;
  }

  // Encrypt sensitive data
  async encryptSensitiveData(data) {
    const sensitiveFields = ['cardToken'];
    const encryptedData = { ...data };

    for (const field of sensitiveFields) {
      if (encryptedData[field]) {
        encryptedData[field] = await this.encryptionKeyManager.encrypt(
          encryptedData[field],
          'payment_card_vault'
        );
      }
    }

    return encryptedData;
  }

  // Network segmentation for payment processing
  isolatePaymentNetwork() {
    return this.networkSegmentation.createSegment({
      name: 'payment_processing',
      allowedProtocols: ['HTTPS'],
      allowedPorts: [443],
      allowedIPs: this.getPaymentGatewayIPs(),
      encryptionRequired: true,
      firewallRules: [
        'DENY all inbound traffic except from payment gateway',
        'ALLOW outbound HTTPS only to payment gateway',
        'LOG all connection attempts'
      ]
    });
  }
}

// PCI DSS API Middleware
const pciMiddleware = {
  // Ensure HTTPS for all payment endpoints
  enforceHTTPS: (req, res, next) => {
    if (!req.secure && req.get('x-forwarded-proto') !== 'https') {
      return res.status(403).json({
        error: 'HTTPS required for payment processing'
      });
    }
    next();
  },

  // Payment request logging
  logPaymentRequest: (req, res, next) => {
    const originalSend = res.send;

    res.send = function(data) {
      // Log payment request (without sensitive data)
      auditLogger.log({
        endpoint: req.path,
        method: req.method,
        timestamp: new Date(),
        requestId: req.id,
        status: res.statusCode,
        // Never log raw request body with card data
        paymentFields: req.body ? Object.keys(req.body).filter(key =>
          !['cardNumber', 'cvv', 'expiryMonth', 'expiryYear'].includes(key)
        ) : []
      });

      return originalSend.call(this, data);
    };

    next();
  },

  // Strong authentication for payment operations
  requireStrongAuth: async (req, res, next) => {
    const authHeader = req.headers.authorization;
    const mfaToken = req.headers['x-mfa-token'];

    if (!authHeader) {
      return res.status(401).json({ error: 'Authentication required' });
    }

    try {
      const user = await authenticateUser(authHeader);
      if (!user) {
        return res.status(401).json({ error: 'Invalid authentication' });
      }

      // Require MFA for payment operations
      if (['POST', 'PUT', 'DELETE'].includes(req.method)) {
        if (!mfaToken) {
          return res.status(401).json({
            error: 'Multi-factor authentication required',
            mfaRequired: true
          });
        }

        const mfaValid = await validateMFAToken(user.id, mfaToken);
        if (!mfaValid) {
          return res.status(401).json({ error: 'Invalid MFA token' });
        }
      }

      req.user = user;
      next();

    } catch (error) {
      return res.status(401).json({ error: 'Authentication failed' });
    }
  },

  // Rate limiting for payment endpoints
  paymentRateLimit: rateLimit({
    windowMs: 15 * 60 * 1000, // 15 minutes
    max: 10, // 10 payment attempts per window
    message: 'Too many payment attempts, please try again later',
    standardHeaders: true,
    legacyHeaders: false
  })
};

// PCI DSS compliant payment API
app.post('/api/payments/charge',
  pciMiddleware.enforceHTTPS,
  pciMiddleware.logPaymentRequest,
  pciMiddleware.requireStrongAuth,
  pciMiddleware.paymentRateLimit,
  async (req, res) => {
    try {
      const paymentData = {
        cardNumber: req.body.cardNumber,
        expiryMonth: req.body.expiryMonth,
        expiryYear: req.body.expiryYear,
        cvv: req.body.cvv,
        amount: req.body.amount,
        currency: req.body.currency,
        merchantId: req.user.merchantId
      };

      const result = await pciCompliance.processPayment(paymentData);
      res.json(result);

    } catch (error) {
      console.error('Payment processing error:', error);
      res.status(400).json({
        error: 'Payment processing failed',
        code: 'PAYMENT_ERROR'
      });
    }
  }
);

// Token retrieval endpoint (returns only token, not card data)
app.post('/api/payments/tokenize',
  pciMiddleware.enforceHTTPS,
  pciMiddleware.requireStrongAuth,
  async (req, res) => {
    try {
      const cardData = {
        cardNumber: req.body.cardNumber,
        expiryMonth: req.body.expiryMonth,
        expiryYear: req.body.expiryYear,
        cvv: req.body.cvv
      };

      const tokenResult = await pciCompliance.tokenizeCardData(cardData);

      res.json({
        cardToken: tokenResult.cardToken,
        cardLast4: tokenResult.cardLast4,
        cardBrand: tokenResult.cardBrand,
        expiryMonth: tokenResult.expiryMonth,
        expiryYear: tokenResult.expiryYear
      });

    } catch (error) {
      res.status(400).json({ error: error.message });
    }
  }
);
```

## HIPAA (Health Insurance Portability and Accountability Act)

### What is HIPAA?

HIPAA is a US federal law that requires the creation of national standards to protect sensitive patient health information from being disclosed without the patient's consent or knowledge.

### HIPAA Compliance for Healthcare APIs

```javascript
// HIPAA Compliance Framework
class HIPAACompliance {
  constructor() {
    this.phiClassifier = new PHIClassifier();
    this.auditLogger = new HIPAAAuditLogger();
    this.accessControl = new AccessControl();
    this.breachNotifier = new BreachNotifier();
    this.businessAssociateManager = new BusinessAssociateManager();
  }

  // Classify and protect Protected Health Information (PHI)
  async processHealthData(data, context) {
    const classification = await this.phiClassifier.classify(data);
    const auditContext = {
      userId: context.userId,
      action: context.action,
      timestamp: new Date(),
      ipAddress: context.ipAddress,
      userAgent: context.userAgent
    };

    try {
      // Check if data contains PHI
      if (classification.containsPHI) {
        // Verify proper authorization
        const hasAuthorization = await this.accessControl.checkAuthorization(
          context.userId,
          context.action,
          classification.phiCategories
        );

        if (!hasAuthorization) {
          await this.auditLogger.logUnauthorizedAccess({
            ...auditContext,
            attemptedAccess: classification.phiCategories,
            reason: 'Insufficient authorization'
          });

          throw new Error('Unauthorized access to PHI');
        }

        // Apply minimum necessary standard
        const minimizedData = await this.applyMinimumNecessary(
          data,
          context.userId,
          context.action
        );

        // Encrypt PHI for storage/transmission
        const encryptedData = await this.encryptPHI(minimizedData);

        // Log the access
        await this.auditLogger.logPHIAccess({
          ...auditContext,
          phiCategories: classification.phiCategories,
          dataMinimized: true,
          encrypted: true
        });

        return {
          data: encryptedData,
          classification,
          requiresEncryption: true,
          accessLevel: this.getAccessLevel(context.userId)
        };

      } else {
        // Non-PHI data, normal processing
        await this.auditLogger.logNonPHIAccess(auditContext);
        return { data, classification, requiresEncryption: false };
      }

    } catch (error) {
      await this.auditLogger.logAccessError({
        ...auditContext,
        error: error.message
      });
      throw error;
    }
  }

  // Apply HIPAA Minimum Necessary Standard
  async applyMinimumNecessary(data, userId, action) {
    const userPermissions = await this.accessControl.getUserPermissions(userId);
    const requiredFields = this.getRequiredFieldsForAction(action, userPermissions);

    const minimizedData = {};
    requiredFields.forEach(field => {
      if (data[field] !== undefined) {
        minimizedData[field] = data[field];
      }
    });

    return minimizedData;
  }

  // Handle patient authorization requests
  async processPatientAuthorization(authorizationRequest) {
    const {
      patientId,
      authorizations,
     有效期: expirationDate,
      purpose,
      revocationKey
    } = authorizationRequest;

    // Validate authorization format
    this.validateAuthorizationFormat(authorizations);

    const authorizationRecord = {
      id: generateAuthorizationId(),
      patientId,
      authorizations,
      purpose,
      createdAt: new Date(),
      expiresAt: new Date(expirationDate),
      revocationKey,
      status: 'active',
      accessLog: []
    };

    // Store encrypted authorization
    await this.storeAuthorization(authorizationRecord);

    // Notify patient of authorization creation
    await this.notifyPatient(patientId, {
      type: 'authorization_created',
      authorizationId: authorizationRecord.id,
      expiresAt: authorizationRecord.expiresAt
    });

    return {
      authorizationId: authorizationRecord.id,
      status: 'active',
      expiresAt: authorizationRecord.expiresAt
    };
  }

  // HIPAA Breach Notification
  async handlePotentialBreach(breachDetails) {
    const breachAssessment = await this.assessBreach(breachDetails);

    if (breachAssessment.isBreach) {
      const breachNotification = {
        id: generateBreachId(),
        discoveredAt: new Date(),
        description: breachDetails.description,
        phiInvolved: breachAssessment.phiCategories,
        affectedIndividuals: breachAssessment.affectedIndividuals,
        notificationTimeline: this.calculateNotificationTimeline(breachAssessment),
        mitigationSteps: breachDetails.mitigationSteps
      };

      // Immediate notification to security officer
      await this.notifySecurityOfficer(breachNotification);

      // If breach affects more than 500 individuals
      if (breachAssessment.affectedIndividuals.length > 500) {
        // Notify within 60 days
        await this.notifyMedia(breachNotification);
        await this.notifySecretary(breachNotification);
      }

      // Notify all affected individuals
      await this.notifyAffectedIndividuals(breachNotification);

      // Log breach notification
      await this.auditLogger.logBreachNotification(breachNotification);

      return breachNotification;
    }

    return { isBreach: false, assessment: breachAssessment };
  }

  // Business Associate Agreement (BAA) management
  async manageBAA(baaRequest) {
    const {
      businessAssociateId,
      permittedUses,
      dataTypes,
      obligations,
      termLength
    } = baaRequest;

    const baaRecord = {
      id: generateBAAId(),
      businessAssociateId,
      permittedUses,
      dataTypes,
      obligations,
      startDate: new Date(),
      endDate: new Date(Date.now() + termLength * 365 * 24 * 60 * 60 * 1000),
      status: 'active',
      complianceChecks: []
    };

    // Validate BAA terms
    this.validateBAATerms(baaRecord);

    // Store BAA
    await this.businessAssociateManager.storeBAA(baaRecord);

    // Setup monitoring for business associate compliance
    await this.setupBAAMonitoring(baaRecord);

    return baaRecord;
  }
}

// PHI Classification System
class PHIClassifier {
  constructor() {
    this.phiFields = new Set([
      // Patient Identifiers
      'patientName', 'firstName', 'lastName', 'fullName',
      'socialSecurityNumber', 'ssn', 'medicalRecordNumber', 'mrn',
      'healthPlanId', 'accountNumber', 'certificateNumber',

      // Geographic Information
      'address', 'street', 'city', 'state', 'zipCode', 'county',
      'precinct', 'geographicCoordinates',

      // Dates
      'birthDate', 'dateOfBirth', 'dob', 'admissionDate',
      'dischargeDate', 'dateOfDeath', 'appointmentDate',

      // Contact Information
      'phoneNumber', 'emailAddress', 'faxNumber', 'url',

      // Medical Information
      'diagnosis', 'diagnosisCodes', 'icd10Code', 'procedures',
      'procedureCodes', 'cptCode', 'medications', 'prescriptions',
      'labResults', 'testResults', 'vitalSigns', 'medicalHistory',
      'allergies', 'conditions', 'symptoms',

      // Insurance Information
      'insuranceProvider', 'policyNumber', 'groupId', 'subscriberId',

      // Treatment Information
      'treatment', 'therapy', 'surgery', 'hospitalization',
      'physician', 'specialist', 'careProvider', 'nurse',

      // Biometric Information
      'fingerprints', 'retinaScan', 'voicePrint', 'dnaSequence'
    ]);
  }

  async classify(dataObject) {
    const classification = {
      containsPHI: false,
      phiCategories: [],
      riskLevel: 'low',
      encryptionRequired: false,
      auditRequired: false
    };

    const detectedFields = [];

    Object.keys(dataObject).forEach(field => {
      const fieldName = field.toLowerCase();
      const fieldValue = String(dataObject[field]).toLowerCase();

      // Check field name and value for PHI indicators
      if (this.isPHIField(fieldName, fieldValue)) {
        detectedFields.push(field);
      }
    });

    if (detectedFields.length > 0) {
      classification.containsPHI = true;
      classification.detectedFields = detectedFields;
      classification.phiCategories = this.categorizePHI(detectedFields);
      classification.riskLevel = this.assessRiskLevel(classification.phiCategories);
      classification.encryptionRequired = true;
      classification.auditRequired = true;
    }

    return classification;
  }

  isPHIField(fieldName, fieldValue) {
    // Check field name
    if (this.phiFields.has(fieldName)) {
      return true;
    }

    // Check for PHI patterns in field name
    const phiPatterns = [
      /name$/i, /address$/i, /phone$/i, /email$/i, /ssn$/i,
      /birthDate$/i, /dob$/i, /diagnosis$/i, /medical$/i,
      /patient$/i, /physician$/i, /treatment$/i
    ];

    for (const pattern of phiPatterns) {
      if (pattern.test(fieldName)) {
        return true;
      }
    }

    // Check for sensitive data patterns in value
    const sensitivePatterns = [
      /^\d{3}-\d{2}-\d{4}$/, // SSN pattern
      /^\d{3}-\d{2}-\d{4}$/, // Another SSN pattern
      /^[A-Z]\d{2}-\d{2}-\d{4}$/, // Medical record number
      /^\d{10,11}$/, // Phone number
      /\d{4}-\d{4}-\d{4}-\d{4}$/ // Credit card (shouldn't be in PHI)
    ];

    for (const pattern of sensitivePatterns) {
      if (pattern.test(fieldValue)) {
        return true;
      }
    }

    return false;
  }

  categorizePHI(detectedFields) {
    const categories = new Set();

    detectedFields.forEach(field => {
      const fieldName = field.toLowerCase();

      if (fieldName.includes('name') || fieldName.includes('ssn') ||
          fieldName.includes('medicalrecord') || fieldName.includes('patient')) {
        categories.add('IDENTIFIER');
      }

      if (fieldName.includes('address') || fieldName.includes('city') ||
          fieldName.includes('state') || fieldName.includes('zip')) {
        categories.add('GEOGRAPHIC');
      }

      if (fieldName.includes('birth') || fieldName.includes('date') ||
          fieldName.includes('admission') || fieldName.includes('discharge')) {
        categories.add('DATES');
      }

      if (fieldName.includes('phone') || fieldName.includes('email') ||
          fieldName.includes('fax')) {
        categories.add('CONTACT');
      }

      if (fieldName.includes('diagnosis') || fieldName.includes('medical') ||
          fieldName.includes('treatment') || fieldName.includes('lab')) {
        categories.add('MEDICAL');
      }

      if (fieldName.includes('insurance') || fieldName.includes('policy') ||
          fieldName.includes('subscriber')) {
        categories.add('FINANCIAL');
      }
    });

    return Array.from(categories);
  }

  assessRiskLevel(phiCategories) {
    // Higher risk if multiple categories are involved
    if (phiCategories.includes('IDENTIFIER') && phiCategories.includes('MEDICAL')) {
      return 'high';
    }

    if (phiCategories.length >= 3) {
      return 'high';
    }

    if (phiCategories.length >= 2) {
      return 'medium';
    }

    return 'low';
  }
}

// HIPAA API Middleware
const hipaaMiddleware = {
  // PHI detection and logging
  phiProtection: async (req, res, next) => {
    try {
      // Classify request data for PHI
      const requestData = { ...req.body, ...req.query, ...req.params };
      const classification = await phiClassifier.classify(requestData);

      // Add classification to request
      req.phiClassification = classification;

      if (classification.containsPHI) {
        // Log PHI access attempt
        await hipaaAuditLogger.logPHIAccess({
          userId: req.user?.id,
          endpoint: req.path,
          method: req.method,
          phiCategories: classification.phiCategories,
          riskLevel: classification.riskLevel,
          timestamp: new Date(),
          ipAddress: req.ip,
          userAgent: req.get('User-Agent')
        });

        // Require additional authentication for high-risk PHI
        if (classification.riskLevel === 'high') {
          const mfaToken = req.get('X-MFA-Token');
          if (!mfaToken) {
            return res.status(401).json({
              error: 'MFA required for high-risk PHI access'
            });
          }

          const mfaValid = await validateMFAToken(req.user.id, mfaToken);
          if (!mfaValid) {
            return res.status(401).json({
              error: 'Invalid MFA token for PHI access'
            });
          }
        }
      }

      next();

    } catch (error) {
      console.error('PHI protection error:', error);
      res.status(500).json({
        error: 'Error processing request'
      });
    }
  },

  // Minimum necessary enforcement
  minimumNecessary: (allowedFields) => {
    return (req, res, next) => {
      if (req.method === 'GET') {
        const originalJson = res.json;

        res.json = function(data) {
          // Filter response to only include necessary fields
          const filteredData = filterDataFields(data, allowedFields);
          return originalJson.call(this, filteredData);
        };
      }

      next();
    };
  },

  // Session timeout for PHI access
  phiSessionTimeout: (req, res, next) => {
    const lastActivity = req.session?.lastPHIActivity;
    const sessionTimeout = 15 * 60 * 1000; // 15 minutes

    if (lastActivity && Date.now() - lastActivity > sessionTimeout) {
      // Session expired for PHI access
      req.session.destroy();
      return res.status(401).json({
        error: 'PHI access session expired',
        requiresReauth: true
      });
    }

    // Update last activity time
    if (req.phiClassification?.containsPHI) {
      req.session.lastPHIActivity = Date.now();
    }

    next();
  }
};

// HIPAA compliant healthcare API
app.get('/api/patients/:id/medical-history',
  authenticateUser,
  hipaaMiddleware.phiProtection,
  hipaaMiddleware.minimumNecessary(['diagnosis', 'medications', 'allergies']),
  hipaaMiddleware.phiSessionTimeout,
  async (req, res) => {
    try {
      const patientId = req.params.id;

      // Check user authorization
      const hasAccess = await hipaaCompliance.checkPHIAccess(
        req.user.id,
        'read',
        'medical_history',
        patientId
      );

      if (!hasAccess) {
        return res.status(403).json({
          error: 'Unauthorized access to patient medical history'
        });
      }

      const medicalHistory = await patientService.getMedicalHistory(patientId);

      res.json({
        patientId,
        medicalHistory,
        accessedAt: new Date().toISOString()
      });

    } catch (error) {
      console.error('Error accessing medical history:', error);
      res.status(500).json({
        error: 'Error retrieving medical history'
      });
    }
  }
);
```

## PII (Personally Identifiable Information)

### What is PII?

PII is any information that can be used to identify, contact, or locate a specific individual, either alone or combined with other sources.

### PII Protection Implementation

```javascript
// PII Management System
class PIIManager {
  constructor() {
    this.piiClassifier = new PIIClassifier();
    this.encryptionService = new EncryptionService();
    this.maskingService = new MaskingService();
    this.retentionManager = new RetentionManager();
    this.auditLogger = new PIIAuditLogger();
  }

  // Classify and protect PII in data
  async processPII(data, context) {
    const classification = await this.piiClassifier.classify(data);
    const processedData = { ...data };

    try {
      // Apply different protection based on classification
      for (const [field, piiType] of Object.entries(classification.piiFields)) {
        const value = processedData[field];

        switch (context.operation) {
          case 'storage':
            // Encrypt for storage
            processedData[field] = await this.encryptPII(value, piiType);
            processedData[field + '_encrypted'] = true;
            break;

          case 'display':
            // Mask for display
            processedData[field] = await this.maskPII(value, piiType, context.userRole);
            processedData[field + '_masked'] = true;
            break;

          case 'logging':
            // Hash for logging (one-way)
            processedData[field] = await this.hashPII(value, piiType);
            processedData[field + '_hashed'] = true;
            break;

          case 'analytics':
            // Anonymize for analytics
            processedData[field] = await this.anonymizePII(value, piiType);
            processedData[field + '_anonymized'] = true;
            break;

          default:
            // Leave as-is if no specific handling required
            break;
        }
      }

      // Log PII processing
      await this.auditLogger.logPIIProcessing({
        operation: context.operation,
        fields: Object.keys(classification.piiFields),
        piiTypes: Object.values(classification.piiFields),
        userId: context.userId,
        timestamp: new Date()
      });

      return {
        data: processedData,
        classification,
        protectionsApplied: this.getAppliedProtections(classification, context)
      };

    } catch (error) {
      await this.auditLogger.logPIIError({
        error: error.message,
        operation: context.operation,
        userId: context.userId,
        timestamp: new Date()
      });
      throw error;
    }
  }

  // PII encryption
  async encryptPII(value, piiType) {
    const encryptionConfig = this.getEncryptionConfig(piiType);
    return await this.encryptionService.encrypt(value, encryptionConfig);
  }

  // PII masking for display
  async maskPII(value, piiType, userRole) {
    return await this.maskingService.mask(value, piiType, userRole);
  }

  // PII data retention management
  async manageRetention(dataRecord) {
    const retentionPeriods = this.getRetentionPeriods(dataRecord.piiTypes);
    const now = new Date();

    for (const [piiType, period] of Object.entries(retentionPeriods)) {
      const expiryDate = new Date(dataRecord.createdAt);
      expiryDate.setFullYear(expiryDate.getFullYear() + period.years);
      expiryDate.setMonth(expiryDate.getMonth() + period.months);

      if (now > expiryDate) {
        await this.expirePII(dataRecord.id, piiType);
      }
    }
  }
}

// PII Classification System
class PIIClassifier {
  constructor() {
    this.piiPatterns = {
      // Direct Identifiers
      socialSecurityNumber: {
        patterns: [
          /^\d{3}-\d{2}-\d{4}$/,
          /^\d{9}$/,
          /^[A-Z]\d{2}-\d{2}-\d{4}$/
        ],
        countries: ['US', 'CA'],
        sensitivity: 'high'
      },

      driversLicense: {
        patterns: [
          /^[A-Z0-9]{8}$/,
          /^\d{8}$/,
          /^[A-Z]{2}\d{6}$/
        ],
        countries: ['US', 'CA', 'UK'],
        sensitivity: 'high'
      },

      passportNumber: {
        patterns: [
          /^[A-Z0-9]{9}$/,
          /^\d{9}$/
        ],
        countries: ['US', 'CA', 'UK', 'EU'],
        sensitivity: 'high'
      },

      email: {
        patterns: [
          /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/
        ],
        countries: ['*'],
        sensitivity: 'medium'
      },

      phoneNumber: {
        patterns: [
          /^\+?1?\d{10}$/,
          /^\d{3}-\d{3}-\d{4}$/,
          /^\(\d{3}\) \d{3}-\d{4}$/
        ],
        countries: ['US', 'CA'],
        sensitivity: 'medium'
      },

      creditCard: {
        patterns: [
          /^4\d{3}[-\s]?\d{4}[-\s]?\d{4}[-\s]?\d{4}$/, // Visa
          /^5[1-5]\d{2}[-\s]?\d{4}[-\s]?\d{4}[-\s]?\d{4}$/, // Mastercard
          /^3[47]\d{2}[-\s]?\d{6}[-\s]?\d{5}$/ // American Express
        ],
        countries: ['*'],
        sensitivity: 'very_high'
      },

      bankAccount: {
        patterns: [
          /^\d{8,17}$/, // US Account
          /^[A-Z]{2}\d{2}[A-Z0-9]{11,30}$/ // IBAN
        ],
        countries: ['*'],
        sensitivity: 'very_high'
      },

      address: {
        patterns: [
          /\d+\s+[\w\s]+,\s*[\w\s]+,\s*[A-Z]{2}\s*\d{5}/,
          /\d+\s+[\w\s]+,\s*[\w\s]+,\s*[A-Z]{1,2}\d{1,2}\s*\d[A-Z]{2}/
        ],
        countries: ['US', 'CA', 'UK'],
        sensitivity: 'medium'
      },

      dateOfBirth: {
        patterns: [
          /^\d{4}-\d{2}-\d{2}$/,
          /^\d{2}\/\d{2}\/\d{4}$/,
          /^\d{2}-\d{2}-\d{4}$/
        ],
        countries: ['*'],
        sensitivity: 'high'
      },

      ipAddress: {
        patterns: [
          /^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/,
          /^(?:[0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4}$/
        ],
        countries: ['*'],
        sensitivity: 'low'
      }
    };

    this.contextPatterns = {
      names: {
        fields: ['firstName', 'lastName', 'fullName', 'middleName', 'surname'],
        sensitivity: 'high'
      },

      identifiers: {
        fields: ['ssn', 'socialSecurity', 'social_security', 'nationalId', 'idNumber'],
        sensitivity: 'high'
      },

      contact: {
        fields: ['phone', 'telephone', 'mobile', 'cell', 'email', 'emailAddress'],
        sensitivity: 'medium'
      },

      location: {
        fields: ['address', 'city', 'state', 'province', 'zip', 'postalCode', 'country'],
        sensitivity: 'medium'
      },

      financial: {
        fields: ['creditCard', 'cardNumber', 'accountNumber', 'bankAccount', 'routingNumber'],
        sensitivity: 'very_high'
      }
    };
  }

  async classify(dataObject) {
    const classification = {
      piiFields: {},
      piiTypes: new Set(),
      overallSensitivity: 'none',
      riskScore: 0,
      jurisdictions: new Set()
    };

    Object.keys(dataObject).forEach(fieldName => {
      const fieldValue = String(dataObject[fieldName]);
      const piiInfo = this.identifyPII(fieldName, fieldValue);

      if (piiInfo.isPII) {
        classification.piiFields[fieldName] = {
          type: piiInfo.type,
          sensitivity: piiInfo.sensitivity,
          confidence: piiInfo.confidence,
          jurisdictions: piiInfo.jurisdictions
        };

        classification.piiTypes.add(piiInfo.type);
        piiInfo.jurisdictions.forEach(j => classification.jurisdictions.add(j));
      }
    });

    // Calculate overall sensitivity and risk score
    classification.overallSensitivity = this.calculateOverallSensitivity(classification.piiFields);
    classification.riskScore = this.calculateRiskScore(classification);

    return classification;
  }

  identifyPII(fieldName, fieldValue) {
    const result = {
      isPII: false,
      type: null,
      sensitivity: 'none',
      confidence: 0,
      jurisdictions: []
    };

    const normalizedName = fieldName.toLowerCase();
    const normalizedValue = fieldValue.toLowerCase();

    // Check field name patterns (higher confidence)
    for (const [category, config] of Object.entries(this.contextPatterns)) {
      if (config.fields.some(field => normalizedName.includes(field))) {
        result.isPII = true;
        result.type = category;
        result.sensitivity = config.sensitivity;
        result.confidence = 0.8;
        break;
      }
    }

    // Check value patterns (pattern matching)
    if (!result.isPII || result.confidence < 0.7) {
      for (const [type, config] of Object.entries(this.piiPatterns)) {
        for (const pattern of config.patterns) {
          if (pattern.test(fieldValue)) {
            const confidence = result.confidence > 0 ? Math.max(result.confidence, 0.6) : 0.6;

            if (confidence > result.confidence) {
              result.isPII = true;
              result.type = type;
              result.sensitivity = config.sensitivity;
              result.confidence = confidence;
              result.jurisdictions = config.countries;
            }
          }
        }
      }
    }

    return result;
  }

  calculateOverallSensitivity(piiFields) {
    const sensitivities = Object.values(piiFields).map(field => field.sensitivity);

    if (sensitivities.includes('very_high')) return 'very_high';
    if (sensitivities.includes('high')) return 'high';
    if (sensitivities.includes('medium')) return 'medium';
    if (sensitivities.includes('low')) return 'low';
    return 'none';
  }

  calculateRiskScore(classification) {
    let score = 0;

    // Base score for having PII
    if (Object.keys(classification.piiFields).length > 0) {
      score += 20;
    }

    // Add points for each PII type
    classification.piiTypes.forEach(type => {
      const typeConfig = this.piiPatterns[type];
      if (typeConfig) {
        switch (typeConfig.sensitivity) {
          case 'very_high': score += 30; break;
          case 'high': score += 20; break;
          case 'medium': score += 10; break;
          case 'low': score += 5; break;
        }
      }
    });

    // Add points for multiple jurisdictions
    score += classification.jurisdictions.size * 5;

    return Math.min(score, 100);
  }
}

// PII Masking Service
class MaskingService {
  mask(value, piiType, userRole) {
    switch (piiType) {
      case 'socialSecurityNumber':
        return this.maskSSN(value, userRole);

      case 'email':
        return this.maskEmail(value, userRole);

      case 'phoneNumber':
        return this.maskPhone(value, userRole);

      case 'creditCard':
        return this.maskCreditCard(value, userRole);

      case 'bankAccount':
        return this.maskBankAccount(value, userRole);

      default:
        return this.maskDefault(value, userRole);
    }
  }

  maskSSN(value, userRole) {
    // Full view for authorized users
    if (['admin', 'hr_manager', 'healthcare_provider'].includes(userRole)) {
      return value;
    }

    // Partial view for limited access
    if (['manager', 'analyst'].includes(userRole)) {
      return `***-**-${value.slice(-4)}`;
    }

    // No view for others
    return '***-**-****';
  }

  maskEmail(value, userRole) {
    const [username, domain] = value.split('@');

    if (['admin', 'customer_service'].includes(userRole)) {
      return value; // Full email
    }

    if (['manager', 'analyst'].includes(userRole)) {
      return `${username.slice(0, 2)}***@${domain}`;
    }

    return `***@${domain}`;
  }

  maskPhone(value, userRole) {
    const cleanNumber = value.replace(/\D/g, '');

    if (['admin', 'customer_service'].includes(userRole)) {
      return value;
    }

    if (['manager', 'analyst'].includes(userRole)) {
      return `***-***-${cleanNumber.slice(-4)}`;
    }

    return '***-***-****';
  }

  maskCreditCard(value, userRole) {
    const cleanNumber = value.replace(/\D/g, '');

    if (['admin', 'finance'].includes(userRole)) {
      return value;
    }

    return `****-****-****-${cleanNumber.slice(-4)}`;
  }
}

// PII API Middleware
const piiMiddleware = {
  // Detect and classify PII in requests
  piiDetection: async (req, res, next) => {
    try {
      const allData = { ...req.body, ...req.query, ...req.params };
      const classification = await piiClassifier.classify(allData);

      req.piiClassification = classification;

      // Log PII detection
      if (Object.keys(classification.piiFields).length > 0) {
        await piiAuditLogger.logPIIDetection({
          endpoint: req.path,
          method: req.method,
          piiTypes: Array.from(classification.piiTypes),
          sensitivity: classification.overallSensitivity,
          riskScore: classification.riskScore,
          userId: req.user?.id,
          timestamp: new Date()
        });
      }

      next();

    } catch (error) {
      console.error('PII detection error:', error);
      next();
    }
  },

  // Apply PII protection based on user role
  piiProtection: (operation) => {
    return async (req, res, next) => {
      try {
        if (!req.piiClassification || Object.keys(req.piiClassification.piiFields).length === 0) {
          return next();
        }

        const processedResult = await piiManager.processPII(
          req.piiClassification.piiFields,
          {
            operation,
            userId: req.user?.id,
            userRole: req.user?.role,
            endpoint: req.path,
            method: req.method
          }
        );

        // Update request/response with protected data
        if (operation === 'display' && req.method === 'GET') {
          const originalJson = res.json;
          res.json = function(data) {
            // Apply masking to response data
            const maskedData = applyPIIProtection(data, processedResult.protectionsApplied);
            return originalJson.call(this, maskedData);
          };
        }

        next();

      } catch (error) {
        console.error('PII protection error:', error);
        next();
      }
    };
  },

  // PII retention enforcement
  retentionCheck: async (req, res, next) => {
    // Add retention headers for PII
    if (req.piiClassification && req.piiClassification.overallSensitivity !== 'none') {
      const retentionPeriod = calculateRetentionPeriod(req.piiClassification);

      res.setHeader('X-PII-Retention-Period', retentionPeriod);
      res.setHeader('X-PII-Sensitivity', req.piiClassification.overallSensitivity);
      res.setHeader('X-PII-Risk-Score', req.piiClassification.riskScore);
    }

    next();
  }
};

// PII compliant API implementation
app.get('/api/users/:id',
  authenticateUser,
  piiMiddleware.piiDetection,
  piiMiddleware.piiProtection('display'),
  piiMiddleware.retentionCheck,
  async (req, res) => {
    try {
      const userId = req.params.id;

      // Check authorization to access user data
      const hasAccess = await checkUserAccess(req.user, userId);
      if (!hasAccess) {
        return res.status(403).json({ error: 'Access denied' });
      }

      const user = await userService.getUser(userId);

      res.json({
        user,
        requestedBy: req.user.id,
        accessedAt: new Date().toISOString()
      });

    } catch (error) {
      console.error('Error fetching user:', error);
      res.status(500).json({ error: 'Error fetching user data' });
    }
  }
);
```

## Compliance Monitoring and Auditing

### Automated Compliance Monitoring

```javascript
// Compliance Monitoring System
class ComplianceMonitor {
  constructor() {
    this.alertManager = new AlertManager();
    this.reportGenerator = new ComplianceReportGenerator();
    this.auditStorage = new AuditStorage();
    this.complianceRules = new ComplianceRules();
  }

  // Real-time compliance monitoring
  async monitorAPIRequest(req, res, next) {
    const startTime = Date.now();
    const requestId = generateRequestId();

    // Capture request details
    const requestContext = {
      id: requestId,
      method: req.method,
      path: req.path,
      headers: this.sanitizeHeaders(req.headers),
      query: req.query,
      params: req.params,
      body: req.body,
      user: req.user,
      timestamp: new Date(),
      ipAddress: req.ip,
      userAgent: req.get('User-Agent')
    };

    try {
      // Check compliance rules before processing
      const preComplianceCheck = await this.complianceRules.checkPreProcessing(requestContext);

      if (!preComplianceCheck.compliant) {
        await this.handleComplianceViolation(preComplianceCheck, requestContext);
        return res.status(403).json({
          error: 'Compliance violation detected',
          rule: preComplianceCheck.violatedRule,
          description: preComplianceCheck.description
        });
      }

      // Continue with request processing
      const originalSend = res.send;

      res.send = function(responseData) {
        // Capture response details
        const responseContext = {
          statusCode: res.statusCode,
          headers: res.getHeaders(),
          dataSize: JSON.stringify(responseData).length,
          processingTime: Date.now() - startTime
        };

        // Check post-processing compliance
        const postComplianceCheck = complianceMonitor.complianceRules.checkPostProcessing(
          requestContext,
          responseContext
        );

        if (!postComplianceCheck.compliant) {
          complianceMonitor.handleComplianceViolation(postComplianceCheck, requestContext);
        }

        // Log the transaction
        complianceMonitor.logTransaction(requestContext, responseContext);

        return originalSend.call(this, responseData);
      };

      next();

    } catch (error) {
      console.error('Compliance monitoring error:', error);
      next();
    }
  }

  // Automated compliance reporting
  async generateComplianceReport(timeframe, regulations) {
    const report = {
      id: generateReportId(),
      timeframe,
      regulations,
      generatedAt: new Date(),
      summary: {},
      violations: [],
      metrics: {},
      recommendations: []
    };

    for (const regulation of regulations) {
      const regulationReport = await this.generateRegulationReport(regulation, timeframe);
      report.summary[regulation] = regulationReport.summary;
      report.violations.push(...regulationReport.violations);
      report.metrics[regulation] = regulationReport.metrics;
    }

    // Generate overall recommendations
    report.recommendations = this.generateRecommendations(report);

    // Store and send report
    await this.reportGenerator.storeReport(report);
    await this.alertManager.sendReport(report);

    return report;
  }

  // GDPR-specific compliance checking
  async checkGDPRCompliance(requestContext) {
    const violations = [];

    // Check for lawful basis of processing
    if (this.containsPersonalData(requestContext)) {
      const hasConsent = await this.checkConsent(requestContext.user?.id, requestContext.path);
      if (!hasConsent) {
        violations.push({
          type: 'missing_consent',
          severity: 'high',
          article: 'Article 6',
          description: 'Processing personal data without lawful basis'
        });
      }
    }

    // Check data minimization
    if (this.exceedsDataMinimization(requestContext)) {
      violations.push({
        type: 'excess_data_collection',
        severity: 'medium',
        article: 'Article 5',
        description: 'Collecting more data than necessary'
      });
    }

    // Check retention policy
    if (this.violatesRetentionPolicy(requestContext)) {
      violations.push({
        type: 'excess_retention',
        severity: 'medium',
        article: 'Article 5',
        description: 'Retaining data longer than necessary'
      });
    }

    return {
      compliant: violations.length === 0,
      violations,
      score: this.calculateComplianceScore(violations)
    };
  }

  // CCPA compliance checking
  async checkCCPACompliance(requestContext) {
    const violations = [];

    // Check for opt-out compliance
    if (requestContext.user?.id) {
      const hasOptedOut = await ccpaCompliance.hasOptedOut(requestContext.user.id);
      if (hasOptedOut && this.isDataSale(requestContext)) {
        violations.push({
          type: 'opt_out_violation',
          severity: 'high',
          section: '1798.120',
          description: 'Processing data sale for opted-out consumer'
        });
      }
    }

    // Check data transparency
    if (!this.providesDataTransparency(requestContext)) {
      violations.push({
        type: 'lack_of_transparency',
        severity: 'medium',
        section: '1798.100',
        description: 'Not providing adequate information about data collection'
      });
    }

    return {
      compliant: violations.length === 0,
      violations,
      score: this.calculateComplianceScore(violations)
    };
  }

  // Handle compliance violations
  async handleComplianceViolation(violation, context) {
    const violationRecord = {
      id: generateViolationId(),
      timestamp: new Date(),
      type: violation.violatedRule,
      severity: violation.severity,
      description: violation.description,
      context: {
        userId: context.user?.id,
        endpoint: context.path,
        method: context.method,
        ipAddress: context.ipAddress
      },
      status: 'open',
      resolved: false
    };

    // Store violation
    await this.auditStorage.storeViolation(violationRecord);

    // Send immediate alerts for high severity violations
    if (violation.severity === 'high') {
      await this.alertManager.sendImmediateAlert(violationRecord);
    }

    // Check if automatic remediation is possible
    const remediation = await this.attemptRemediation(violation, context);
    if (remediation.success) {
      violationRecord.resolved = true;
      violationRecord.resolution = remediation.description;
    }

    return violationRecord;
  }
}

// Comprehensive compliance middleware
const complianceMiddleware = {
  // Apply all compliance checks
  comprehensive: (regulations = ['GDPR', 'CCPA', 'PCI_DSS', 'HIPAA']) => {
    return async (req, res, next) => {
      const monitor = new ComplianceMonitor();
      await monitor.monitorAPIRequest(req, res, next);
    };
  },

  // GDPR specific middleware
  gdpr: gdprMiddleware.phiProtection,

  // CCPA specific middleware
  ccpa: ccpaMiddleware.checkOptOut,

  // PCI DSS specific middleware
  pci: pciMiddleware.enforceHTTPS,

  // HIPAA specific middleware
  hipaa: hipaaMiddleware.phiProtection,

  // PII protection middleware
  pii: piiMiddleware.piiDetection
};

// Apply compliance monitoring to all APIs
app.use('/api/*',
  authenticateUser,
  complianceMiddleware.comprehensive(['GDPR', 'CCPA', 'HIPAA', 'PCI_DSS'])
);
```

## Interview Questions

### Basic Questions

1. **What is GDPR and why is it important for APIs?**
   - EU regulation protecting personal data
   - Requires consent, data minimization, breach notifications
   - Applies to any API processing EU user data

2. **What's the difference between PII and PHI?**
   - PII: Any personally identifiable information
   - PHI: Protected health information under HIPAA
   - PHI is a subset of PII with additional protections

3. **What is PCI DSS compliance?**
   - Security standards for handling credit card data
   - Required for any API processing payment information
   - Includes encryption, access controls, and network security

### Intermediate Questions

4. **How do you implement data minimization in APIs?**
   - Return only necessary data fields
   - Implement field-level permissions
   - Use data filtering middleware
   - Validate data requirements

5. **What are the key requirements for CCPA compliance?**
   - Right to know what data is collected
   - Right to delete personal information
   - Right to opt-out of data sales
   - Transparency about data practices

6. **How do you handle data subject requests under GDPR?**
   - Implement secure verification process
   - Provide data access within 30 days
   - Support erasure, rectification, portability
   - Log all requests and responses

### Advanced Questions

7. **How do you implement cross-jurisdictional compliance?**
   - Detect user location and apply appropriate laws
   - Implement data localization requirements
   - Use different data retention periods by region
   - Handle international data transfer mechanisms

8. **What strategies do you use for HIPAA compliance in healthcare APIs?**
   - Minimum necessary standard
   - Business associate agreements
   - Audit logging for all PHI access
   - Secure communications and encryption

9. **How do you automate compliance monitoring?**
   - Real-time API request monitoring
   - Automated compliance rule checking
   - Regular compliance report generation
   - Alert system for violations

### Scenario-Based Questions

10. **How would you handle a data breach affecting EU citizens?**
    - Immediate assessment and containment
    - Notify authorities within 72 hours if high risk
    - Notify affected individuals without undue delay
    - Document breach and response actions

11. **How would you implement the "Right to be Forgotten"?**
    - Delete personal data from all systems
    - Notify third parties of deletion request
    - Implement automated deletion workflows
    - Maintain records of deletion requests

12. **How would you secure payment card data in your APIs?**
    - Never store raw card numbers
    - Use tokenization services
    - Implement network segmentation
    - Regular PCI DSS compliance audits

## Summary

API compliance is crucial for legal operation and customer trust. Key areas include:

- **GDPR**: EU data protection with rights to access, delete, and portability
- **CCPA**: California privacy rights including opt-out of data sales
- **PCI DSS**: Payment card security standards for financial APIs
- **HIPAA**: Healthcare data protection with minimum necessary standard
- **PII Protection**: General personally identifiable information security

Success factors:
- Implement privacy by design principles
- Use automated compliance monitoring
- Maintain comprehensive audit trails
- Regular compliance assessments and updates
- Clear data handling policies and procedures

Remember: Compliance is not a one-time implementation but an ongoing process requiring continuous monitoring, updates, and employee training.