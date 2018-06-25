{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "addon.cc", "mbrtu.cpp", "ModbusRTU.cpp", "serial.cpp"],
      "msbuild_settings": {
        "ClCompile": {
          "CompileAsManaged": "true",
          "ExceptionHandling": "Async",
        },
      },
    }
  ]
}
