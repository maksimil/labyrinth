workspace "Spbu"
   configurations { "Debug", "Release" }

project "a"
   kind "ConsoleApp"
   language "C"
   files { "main.c" }

   filter { "configurations:Debug" }
      defines { "DEBUG" }
      symbols "On"

   filter { "configurations:Release" }
      defines { "NDEBUG" }
      optimize "On"

project "rational-roots"
      kind "ConsoleApp"
      language "C"
      files { "rational-roots.c" }
      filter { "configurations:Debug" }
         defines { "DEBUG" }
         symbols "On"

      filter { "configurations:Release" }
         defines { "NDEBUG" }
         optimize "On"


