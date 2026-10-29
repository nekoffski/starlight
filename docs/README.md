# Repository knowledge

These knowledge nodes describe the code currently in this repository:

- [Build and setup](../README.md): prerequisites and Makefile workflow.
- [Renderer design](renderer-design.md): frame flow, resources, and backend
  boundaries, grounded in the linked implementation.
- [Renderer messaging](renderer-messaging.md): payload ownership, replies,
  resource lifetime, and shutdown behavior in the current command path.
- [Shader pipeline](shader-pipeline.md): compilation, reflection, vertex layout,
  asset wrappers, and indexed texture/sampler binding.
- [Renderer language](CONTEXT.md): glossary for the implemented renderer concepts.
- [Implementation status](roadmap.md): source-backed renderer and platform
  status. The filename is retained from the earlier roadmap.

## Maintaining knowledge nodes

Do not create ADRs (Architecture Decision Records). Update documentation inside
`/docs` as development changes the implementation. Ideas, design descriptions,
and code interface notes must reflect the repository's actual code, rather than
drive development from speculative design.

Read the implementation and callers, then update the affected node in the same
change. Link claims to source files. Remove or correct stale descriptions; do
not add features, interfaces, guarantees, or rationale that the code does not
support. A declaration, TODO, or empty stub is not evidence of working support.
If a fact cannot be verified, omit it and report the uncertainty instead.
