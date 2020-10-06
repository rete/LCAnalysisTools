


import sys
try:
    from particle import Particle
except ImportError:
    print ("Couldn't import particle.\nInstall with: 'pip install particle [--user]'")

def particleToStr( part ):
    """ Dumps a particle into a C++ ParticleData object understandable format
        Returns its string representation
    """
    digit_str = str(abs(int(part.pdgid)))
    conc = ", " if len(digit_str) != 10 else ""
    digits = "{ " + ", ".join(["-1"] * (10-len(digit_str))) + conc + ", ".join(list(digit_str)) + " }"
    particle_properties = [ 
        str(int(part.pdgid)),
        "std::nullopt" if part.mass is None else str(part.mass),
        "std::nullopt" if part.mass_upper is None else str(part.mass_upper),
        "std::nullopt" if part.mass_lower is None else str(part.mass_lower),
        "std::nullopt" if part.width is None else str(part.width),
        "std::nullopt" if part.width_upper is None else str(part.width_upper),
        "std::nullopt" if part.width_lower is None else str(part.width_lower),
        "std::nullopt" if part.I is None else str(part.I),
        str(int(part.G)),
        str(int(part.C)),
        digits,
        "\"" + part.name + "\""
    ]
    return "      ParticleData( { " + ", ".join(particle_properties) + " } )"


if __name__ == "__main__":
    
    # Get the output c++ header file name
    fname = "PDGTable.cc"
    if len(sys.argv) > 1:
        fname = sys.argv[1]
    
    # Get all particle from the PDG table
    allParticles = Particle.findall()
    # Convert each particle to c++ code strings
    particlesStr = map( particleToStr, allParticles )

    # Open the header file and write the list in a c++ vector
    f = open( fname, 'w' )
    f.write( "\n\n" )
    f.write( "#include <vector>\n" )
    f.write( "#include <optional>\n" )
    f.write( "#include <LCAnalysisTools/PDGHelper.h>\n" )
    f.write( "\n" )
    f.write( "namespace lc_analysis {\n" )
    f.write( "  namespace pdg {\n" )
    f.write( "    const std::vector<ParticleData> pdgTable = {\n" )
    f.write( ",\n".join( particlesStr ) + "\n" )
    f.write( "    };\n" )
    f.write( "  }\n" )
    f.write( "}\n" )
    f.close()
    
    print ("Output written in '%s'"%( fname ))




