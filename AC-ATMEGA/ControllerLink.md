# Controller UART link

Mega controls hardware and holds configuration in RAM. ESP owns external networking,
external field mapping, the web panel, discovery and durable configuration.

See [WireProtocol.md](../AC-ESP/WireProtocol.md) for numeric messages, CRC,
acknowledgements, retransmission, startup gates and persistence.

`ControllerLink.ino` applies validated controller settings and produces paced snapshots.
`Wire.ino` sends typed Value/Result/Request/Power/Diagnostic messages directly.
`WireFields.h` contains numeric IDs and channel helpers; field names exist only on ESP.
External field names live on ESP. Numeric IDs are regenerated with the shared schema; update both firmwares together after a schema change.

The LCD receives WiFi and broker status as metadata from ESP. Broker credentials
and external command routing remain on ESP.

ESP supplies a validated runtime capability manifest before restoring active settings.
Mega retains support for all hardware; disabled channels stay off and their fields
are filtered from snapshots. Changed capabilities close output interlocks until the
new configuration is verified. See [FEATURES.md](../AC-ESP/FEATURES.md).
