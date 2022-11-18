workspace "Spbu"
   configurations { "Debug", "Release" }

project "labyrinth"
   kind "ConsoleApp"
   language "C"
   files { "*.c", "*.h" }

   filter { "configurations:Debug" }
      defines { "DEBUG" }
      symbols "On"

   filter { "configurations:Release" }
      defines { "NDEBUG" }
      optimize "On"

