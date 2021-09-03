# Salieri : A Polyvalent Serialization Library

## Binary Serialization

WIP

## Json Serialization

You must first create a Doc :
```cpp
slr::json::Doc doc;
```

To save an object into a Doc : 
```cpp
MyObject obj = ... ; // Construct and initialize obj with custom values
doc.save(obj, {"obj"}); // save obj into the doc
```

To load an object from a Doc : 
```cpp
MyObject obj; // Construct new instance of MyObject
doc.load(obj, {"obj"}); // Load values into an already existing instance of MyObject
```

To save a Doc to a File :
```cpp
slr::File file = "mySaveFolder/mySaveFile.json"; // Open the mySaveFile.json file in the mySaveFolder folder
file << doc; // Save the Doc content into the file
```

To load a Doc from a file :
```cpp
slr::File file = "mySaveFolder/mySaveFile.json"; // Open the mySaveFile.json file in the mySaveFolder folder
file >> doc; // Load the file content into the Doc
```

Doc conversions :
```cpp
doc.toJson();
doc.toBson();
doc.toCbor();
doc.toMessagePack();
doc.toUbjson();

doc.fromJson(...);
doc.fromBson(...);
doc.fromCbor(...);
doc.fromMessagePack(...);
doc.fromUbjson(...);
```

Network : WIP

## Imgui Serialization

WIP
