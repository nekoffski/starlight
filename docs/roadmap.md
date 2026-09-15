## Roadmap

## Renderer

- [ ] separate renderer thread architeture, main thread communicates via commands
- [ ] slang shader lang support


## Runtime/Threading

- [ ] implement simple reactor
- [ ] thread pool

## Backlog

### Renderer

- [ ] use lock-less queue for renderer requests
- [ ] interface for destroying surface

### Adhoc improvements

- [ ] use some dedicated lease type instead of shared pointer to avoid double unwrap when returning Result<..>
- [ ] flat map

