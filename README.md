# cDataPipeline

C Implementation of the FAIR Data Pipeline API. This library wraps the 
[C++ implementation](https://github.com/FAIRDataPipeline/cppDataPipeline), which is
included as a Git submodule. Please consult the documentation for the C++ implementation
for information on dependencies and logging.

## Installation

To use this library, clone using:

```bash
$ git clone --recursive https://github.com/PlasmaFAIR/cDataPjpeline
```

This will also include a simple
[SEIRS model](https://github.com/PlasmaFAIR/cDataPipelineSimpleModel) used for testing.

To build this library:

```bash
$ cmake -B build
$ cmake --build build
```

To install it to your system:

```bash
$ sudo cmake --build build --target install
```

## Testing

Prior to testing, the user should set up a FAIR Data Pipeline registry. To begin,
install the [FAIR-CLI](https://github.com/FAIRDataPipeline/FAIR-CLI):

```bash
$ pip install fair-cli
```

Then start a local registry:

```bash
$ fair registry install
$ fair registry start
```

Note the URL on which the server is running, which should be something like
`http://127.0.0.1:8000`. Then, initialise the repository:

```
$ fair init
```

The default inputs will work in most cases, but you may need to set the 'Remote API'
using the URL reported by the previous command, i.e. `http://127.0.0.1:8000/api/`.
You may also need to set the registry token, which should be at
`/home/USERNAME/.fair/registry/token`, where `USERNAME` is your login name.

To build and run tests:

```bash
$ sudo cmake --build build --target tests
```
