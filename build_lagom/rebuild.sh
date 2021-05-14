cd $(dirname "$0")

if [ "$(basename `pwd`)" != "build_lagom" ]; then
  echo "You're running this in the wrong directory."
  echo "There will be some automatic rm magic"
  echo "This is highly disencouraged."
  exit 1
fi


rm -r AK CMakeFiles Kernel Meta Root Tests Userland
rm *.txt *.ninja *.cmake *.json

cmake -GNinja -DBUILD_LAGOM=on -DENABLE_FUZZER_SANITIZER=ON -DENABLE_ADDRESS_SANITIZER=ON -DENABLE_UNDEFINED_SANITIZER=ON -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang ..
