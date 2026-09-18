# NBP Integration into bootpd

## Status
**Geplant** - 2026-09-18

## Ziel
Das bestehende NBP-Projekt (`/mnt/openclaw/projects/nbp/`) in bootpd als Plugin integrieren.

## Bestehende NBP-Struktur

```
/mnt/openclaw/projects/nbp/
├── CMakeLists.txt
├── README.md
├── src/
│   ├── main.c        # UI, Stubs für Netzwerk
│   └── nbp.S         # Minimal 16-bit Assembly Entry
└── include/          # (leer)
```

## Was NBP kann (aktuell)
- Screen clear (BIOS)
- String print (BIOS)
- Progress bar (ASCII)
- Boot menu (mock)
- **Network: NOT IMPLEMENTED**

## Was NBP können sollte (PXE Spec)
- PXE API nutzen (!PXE / PXENV+)
- DHCP/DISCOVER Pakete via `PXENV_GET_CACHED_INFO`
- TFTP Download via `PXENV_TFTP_*`
- UDP Kommunikation via `PXENV_UDP_*`
- UNDI für Netzwerk-I/O

## Integration in bootpd

### Option 1: Eigenes NBP-Plugin
```
bootpd/plugins/nbp/
├── CMakeLists.txt
├── NBP.h
├── NBP.cpp           # Wrapper + PXE API calls
├── pxenv.S           # PXE Structs (aus Spec)
└── tftp.c            # TFTP Client
```

### Option 2: NBP als Submodul
NBP als Git-Submodul in bootpd einbinden.

### Option 3: Gemeinsames Build-System
NBP + bootpd zusammen bauen.

## Nächste Schritte

1. [ ] NBP-Sourcen in bootpd/plugins/nbp/ kopieren
2. [ ] CMakeLists.txt für NBP-Plugin erstellen
3. [ ] PXE API Wrapper implementieren
4. [ ] TFTP Client für NBP download
5. [ ] BIS-Integration (VerifyBootObject)
6. [ ] Test mit bootpd DHCP/TFTP

## Referenzen
- PXE Spec: `/tmp/pxespec.txt`
- BIS Spec: `/tmp/bisspec.txt`
- bootpd Plan: `docs/BIS_IMPLEMENTATION_PLAN.md`
