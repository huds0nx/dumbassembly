# Disclaimer
This is not my code, I just uploaded it here because I couldn't find it anywhere else.

The original author of the code is [arc_](http://www.woodmann.com/forum/memberlist.php?mode=viewprofile&u=19806)

## tl;dr
dumbassembly is a smartassembly decompiler.

### Original description
A while ago I decided to get my hands dirty on .NET protections, and {smartassembly} by redgate seemed like a nice start. There already exist unpackers for this, but they seem to be mostly out of date (don't support the newer protections), and more importantly I was unable to find an explanation on how the protection actually works.

So I set out and explored it by myself. It turns out there is really not that much to this "first-rate obfuscator" as redgate calls it. The protection consists of these layers:
* The names of non-public namespaces, classes, methods and fields are removed and replaced by junk. Not much to do against that.
* All the string constants are encrypted. ldstr instructions get replaced by a call to a method that fetches the string by index.
* All calls to imported methods (methods from externally referenced types) are obfuscated. They get replaced by a call to a delegate that passes control to the original imported method. This delegate is set up at runtime.
* Some weak control flow obfuscation is performed. It's a bit like the strategic code splicing as seen in Armadillo: small code sequences of one or two instructions get moved to the end of the function, and a branch is placed at their original location.
* Finally, all the resources are encrypted as well (but their names are left alone, e.g. in instantiations of ResourceManager).
That's it. Now let's look at these in a bit more detail.

#### String encryption
Every string literal is encoded as UTF-8 and then base64. It also gets a length prefix (which is variable-length encoded). All these length-prefixed base64 strings are then concatenated - the resulting block is then compressed using regular deflate(), and encrypted using regular DES.

The encrypted block is stored in the binary as a resource stream (with a GUID as name) and is decrypted and decompressed at runtime. This process consists of a loop which performs a deobfuscation pass on every iteration, passing the output of each pass as input to the next. It identifies the next layer to remove by a 4-byte header that is prepended to the data:
* "{z}\0x02": decrypt the data using DES (key and IV are in plaintext in the binary)
* "{z}\0x01": decompress the data using raw, headerless inflate
Every class that uses the ldstr instruction somewhere has a static field added which contains a delegate. Every ldstr instruction gets replaced by a call to this delegate. It accepts an int and returns the corresponding string through the following algorithm:
* Subtracts its own metadata field index from the given number (i.e. the lower 24 bits of the metadata token)
* Subtracts a fixed constant, which is stored as plaintext in the #US stream
The resulting number is an offset into the base64 block. It goes to that offset, reads the variable-length encoded string length, extracts the base64 string, decodes from base64, and decodes from UTF-8. Optionally the int -> string mapping is cached.
Imported methods
The actual imports (MemberRef metatable) are left completely intact, only the calls to them are obfuscated.

For every import method signature, a new delegate class is added to the binary (with no namespace). For every imported method, a static field is added to its corresponding delegate class, containing an instance of that class. Every call to an imported method is replaced with a call to that static delegate instance.

The delegate instances are initialized at runtime in the delegate class's static constructor. They get assigned a method body that is generated using reflection, which does nothing more but pass through the parameters to the original imported method. The index of the imported method to call (in the MemberRef table) is calculated from the name of the delegate instance field. The name also indicates whether the call is virtual or not.

#### Code flow obfuscation
Not much more to say about this than has already been said. The chosen code sequences get moved to the end of the innermost exception handler, or, if there isn't one, to the end of the method. This is not done very often in each method (most of the code is left unmodified) and no further junk code is added.

I solved this by building a basic block tree of the method and from there searching for "redundant" branches: unconditional branches to a basic block that is not reachable from anywhere else. I then remove the branch and "pull in" the target basic block to after the basic block that jumps to it.

#### Resources
All resource streams are moved out of the main file into a separate, new assembly that has a GUID as name. This assembly is then compressed and encrypted in exactly the same way and with the same keys as the strings block, and is also added as a resource stream with a GUID name (the same as the assembly name).

The <Method> .cctor is extended with code that registers two event handlers: ResourceResolve and AssemblyResolve.

Since the original resources were removed, any attempt to access them will result in the ResourceResolve event being triggered. The handler for this will do Assembly.Load with the name of the resource assembly (the GUID), and checks if this can serve the resource request. If so, it returns the assembly.

Of course, the resource assembly can't be loaded just like that since it doesn't actually exist as a .dll on disk. This is where the AssemblyResolve handler comes in. It compares the name of the requested assembly to the name of the resource assembly, and if this matches, it decrypts, decompresses, loads, and returns the resource assembly, all in-memory.

#### The unpacker
I couldn't come up with a clever name for my {smartassembly} unpacking tool so I just called it dumbassembly 😝. It removes all code protections mentioned here except the destroyed names. It also decrypts and extracts the resources. I do not claim that it will work on all {smartassembly} protected applications. In fact, I only tested it on one (another redgate product). In the end this project was more about analyzing the protection and demonstrating how it works.

The unpacker is completely free and open source and also comes with the description of the different protection layers. So if you find a binary which the unpacker doesn't work on, you have the possibility of analysing and fixing the problem yourself.

Since I generally can't stand .NET applications because of their typical sluggishness, it's written in (optimized) C++ 😁. A Windows binary is included with the download. If you want to compile it yourself, be aware that 1) it is Windows-only and 2) will only compile with VS2010 as it uses some C++0x features.

#### RebelDotNET
The unpacker comes with RebelDotNET by Daniel Pistelli (http://ntcore.com/), an excellent .NET PE rebuilder. It uses it for replacing the #US stream with a new one containing all the decrypted strings.
