# bootpd - BIS/PXE Implementation Plan

## Overview

**Goal:** Ein bootfähiger Netzwerk-Server (DHCP/TFTP) mit BIS-Sicherheit und PXE-Protokoll-Unterstützung.

```
┌─────────────────────────────────────────────────────────────┐
│                        bootpd Core                           │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐ │
│  │   Plugin    │  │   Event     │  │    Service          │ │
│  │   Loader   │  │   Bus       │  │    Registry         │ │
│  └─────────────┘  └─────────────┘  └─────────────────────┘ │
├─────────────────────────────────────────────────────────────┤
│                   Plugin Interfaces                          │
│  ┌──────────┐ ┌──────────┐ ┌──────────┐ ┌───────────────┐ │
│  │IDHCP     │ │ITFTP     │ │IBoot     │ │ISecurity     │ │
│  │Handler   │ │Server    │ │Verifier  │ │(BIS)         │ │
│  └──────────┘ └──────────┘ └──────────┘ └───────────────┘ │
├─────────────────────────────────────────────────────────────┤
│                     Plugins (dynamisch)                     │
│  ┌────────┐ ┌────────┐ ┌────────┐ ┌────────┐ ┌─────────┐ │
│  │  DHCP  │ │  TFTP  │ │  DNS   │ │  NTP   │ │  BIS-   │ │
│  │Service │ │Server  │ │Forwarder│ │Server  │ │  Core   │ │
│  └────────┘ └────────┘ └────────┘ └────────┘ └─────────┘ │
│  ┌────────┐ ┌────────┐ ┌────────┐ ┌────────┐ ┌─────────┐ │
│  │  WDS   │ │  RIS   │ │  BSDP  │ │  PXE   │ │   NBP   │ │
│  │Support │ │Support │ │Support │ │Menu    │ │(Boot)   │ │
│  └────────┘ └────────┘ └────────┘ └────────┘ └─────────┘ │
└─────────────────────────────────────────────────────────────┘
```

---

## Intel Referenz-Code (verfügbar!)

### Von Guido bereitgestellt:
- **bisserver.zip** - BIS Server Komponenten (Windows/Linux)
  - `/tmp/bisserver/BISServerComponents/`
  
- **bisclient_a.zip** - BIS Client Komponenten
  - `/tmp/bisclient/BISClientComponents/`

### Architektur:
```
┌─────────────┐         UDP         ┌─────────────────┐
│  BIS Client │ ◄─────────────────► │   BIS Server    │
│  (PXE ROM)  │   Port 69/4011      │  (bootpd)       │
└─────────────┘                      └─────────────────┘
      │                                     │
      │ BIS_VerifyBootObject()              │ BIS_GetBootObjectAuthCertificate()
      │ BIS_GetBOA_CheckFlag()              │ BIS_UpdateBootObjectAuth()
      ▼                                     ▼
┌─────────────┐                      ┌─────────────────┐
│ Boot Image  │                      │   Credentials   │
│ Verification│                      │   (Certificates│
└─────────────┘                      └─────────────────┘
```

---

## PXE Boot Flow (Spec 2.1)

```
1. PXE Client bootet → DHCPDISCOVER (PXEClient Tag #60)
                          │
                          ▼
2. Proxy DHCP / DHCP Server antwortet
   - Client IP (wenn DHCP)
   - Boot Server Liste (PXE Option 43)
   - Boot File Name (oder BStrap.0)
                          │
                          ▼
3. Client bootet von Network
   - TFTP Download: NBP (Network Bootstrap Program)
   - Download nach 0:7C00 (x86)
                          │
                          ▼
4. PXE ROM übergibt an NBP
   - FAR CALL zu 0:7C00
   - ES:BX = PXENV+ Struktur
   - SS:[SP+4] = !PXE Struktur
                          │
                          ▼
5. NBP nutzt PXE APIs:
   - TFTP API: Dateien laden
   - UDP API: Netzwerk-Kommunikation
   - UNDI API: Netzwerk-Treiber
   - Cached Info: DHCP Pakete holen
                          │
                          ▼
6. OPTIONAL: BIS Verifikation
   - BIS_GetBootObjectAuthorizationCheckFlag()
   - Wenn Flag=TRUE → Credentials vom Server holen
   - BIS_VerifyBootObject(NBP, Credentials)
   - Wenn Fehler → Boot verweigert
```

---

## NBP (Network Bootstrap Program)

### Aktuelles NBP (`/mnt/openclaw/projects/nbp/`):
- **Status:** UI funktioniert, Netzwerk NICHT implementiert
- **Features:**
  - Screen clear (BIOS)
  - String print (BIOS)
  - Progress bar (ASCII)
  - Boot menu (mock)
  - **Network: STUBS ONLY**

### NBP Spezifikation (PXE Spec):
```
Loading Address: 0:7C00h (ganzer NBP)
Entry: FAR CALL von PXE ROM
Stack: mindestens 1.5KB frei

PXE Structs:
- PXENV+ (Legacy, für alte NBPs)
- !PXE (Neu, API Version >= 2.1)

Memory Map beim Boot:
0x00000 - Interrupt Vektoren
0x00400 - BIOS Data Segment  
0x7C00  - NBP Download-Adresse!
0x8D000 - PXE Stack (1.5KB min)
0x8D800 - BC Data Segment
0x91800 - BC Code Segment
0x97800 - UNDI Data Segment
0x9B800 - UNDI Code Segment
```

---

## BIS API Referenz (from Intel Code)

### Client APIs (`bis/API/`):
```c
// Initialisierung
BIS_Initialize()         // BIS Handle holen
BIS_Free()              // Handle freigeben
BIS_Shutdown()          // BIS deaktivieren

// Boot Object Verification  
BIS_VerifyBootObject()  // Boot-Image verifizieren (KERNEL!)
BIS_GetBootObjectAuthorizationCertificate()
BIS_GetBootObjectAuthorizationCheckFlag()
BIS_UpdateBootObjectAuthorization()

// Signatur Info
BIS_GetSignatureInfo()
```

### Server APIs (`BISServer/BISInterface/`):
```cpp
BSL_GetBootObjectAuthorizationCertificate()  // Cert an Client
BSL_GetBootObjectAuthorizationCheckFlag()    // Flag setzen
BSL_UpdateBootObjectAuthorization()          // Policy updaten
BSL_TerminateClient()                       // Client beenden
```

---

## Plugin-Struktur

```
bootpd/plugins/
├── bis-core/           ✅ existiert (PoC)
│   ├── src/BISCore.h/cpp
│   └── ISecurityInterfaces.h
├── dhcp/               ⚠️ hat Build-Probleme
├── nbp/                📋 zu integrieren
│   ├── src/main.c      (existiert)
│   ├── src/nbp.S      (existiert)
│   └── src/pxe_api.c  (NEU - PXE API Wrapper)
├── pxe/                📋 NEU
│   ├── src/PXEServer.h/cpp
│   ├── src/DHCPPxeHandler.cpp
│   └── src/TFTPDownloader.cpp
└── bis-server/         📋 NEU (aus Intel Referenz)
    ├── src/BISServerHandler.cpp
    ├── src/CredentialManager.cpp
    └── src/ClientSession.cpp
```

---

## Implementation Phasen

### Phase 1: Core Infrastructure ✅
- [x] Plugin System (ServiceManager)
- [x] Plugin Loader
- [x] Event Bus Interface
- [x] Service Registry Interface

### Phase 2: DHCP + TFTP (Bestehend)
- [ ] DHCP Plugin fixen (Include-Pfade)
- [ ] TFTP Plugin implementieren
- [ ] PXE Options (43, 60, 66, 67) in DHCP

### Phase 3: PXE Support (NEU)
- [ ] PXE Server Plugin
- [ ] Boot Server Discovery
- [ ] MTFTP Support
- [ ] NBP Integration

### Phase 4: NBP Boot Program
- [ ] NBP aus `/mnt/openclaw/projects/nbp/` integrieren
- [ ] PXE API Wrapper implementieren
- [ ] TFTP Client für NBP Download
- [ ] UNDI API Stub

### Phase 5: BIS Security (aus Intel Referenz)
- [ ] BIS-Core Plugin erweitern
- [ ] BIS Server aus Referenz-Code adaptieren
- [ ] Credential Management
- [ ] VerifyBootObject Integration

---

## Konfiguration (YAML)

```yaml
server:
  bind_address: 0.0.0.0
  port: 67
  
plugins:
  - name: dhcp
    enabled: true
    config:
      range_start: 10.0.0.100
      range_end: 10.0.0.250
      subnet_mask: 255.255.255.0
      boot_file: nbp
  
  - name: pxe
    enabled: true
    config:
      boot_server_types:
        - type: 0  # BS_Normal
          name: "Boot"
        - type: 4  # BS_WindowsBootEnv
          name: "Windows PE"
      menu_prompt: "Select boot option"
      menu_timeout: 10
  
  - name: bis-core
    enabled: true
    config:
      allow_unverified: false
      require_secure_boot: true
      policy: enforce
  
  - name: bis-server
    enabled: true
    config:
      certificate_path: /etc/bootpd/certs/ca.pem
      private_key_path: /etc/bootpd/certs/key.pem
      credential_timeout: 300
```

---

## Nächste Schritte (Sofort)

1. **NBP Projektstruktur checken**
   ```bash
   ls -la /mnt/openclaw/projects/nbp/
   cat /mnt/openclaw/projects/nbp/src/main.c
   ```

2. **DHCP Plugin Build fixen** (bekannte Include-Pfade)

3. **PXE API Wrapper für NBP schreiben**
   - `pxe_api.h` - PXE Structs aus Spec
   - `pxe_api.c` - TFTP/UDP/UNDI Stubs

4. **Intel Referenz-Code analysieren**
   - BIS Server Components in bootpd struktur einbauen
   - Client Code für NBP anpassen

---

## Referenzen

- PXE Spec: `/tmp/pxespec.txt` (103 pages, Intel 1999)
- BIS Spec: `/tmp/bisspec.txt` (64 pages, Intel 1998)
- Intel Referenz:
  - Server: `/tmp/bisserver/BISServerComponents/`
  - Client: `/tmp/bisclient/BISClientComponents/`

---

*Letzte Änderung: 2026-09-18*
*Status: Planning + Intel Reference Code verfügbar*
