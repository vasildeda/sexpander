export CMAKE_BUILD_PARALLEL_LEVEL=$(sysctl -n hw.ncpu)
cmake --build build && AudioPluginHost.app/Contents/MacOS/AudioPluginHost Plugin.filtergraph
