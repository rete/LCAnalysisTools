


import sys
try:
    from particle import Particle
except ImportError as e:
    print ("Couldn't import particle.\nInstall with: 'pip install particle [--user]'")
    raise e

def particleToStr( part ):
    """ Dumps a particle into a comma separated properties
        Returns its string representation
    """
    digit_str = str(abs(int(part.pdgid)))
    conc = ", " if len(digit_str) != 10 else ""
    digits = "{ " + ", ".join(["-1"] * (10-len(digit_str))) + conc + ", ".join(list(digit_str)) + " }"
    particle_properties = [ 
        str(int(part.pdgid)),
        "" if part.mass is None else str(part.mass),
        "" if part.mass_upper is None else str(part.mass_upper),
        "" if part.mass_lower is None else str(part.mass_lower),
        "" if part.width is None else str(part.width),
        "" if part.width_upper is None else str(part.width_upper),
        "" if part.width_lower is None else str(part.width_lower),
        "" if part.I is None else str(part.I),
        str(int(part.G)),
        str(int(part.C)),
        # digits,
        "\"" + part.name + "\""
    ]
    return ",".join(particle_properties)


if __name__ == "__main__":
    
    # Get the output c++ header file name
    fname = "pdg-table.txt"
    if len(sys.argv) > 1:
        fname = sys.argv[1]
    
    # Get all particle from the PDG table
    allParticles = Particle.findall()
    # Convert each particle to c++ code strings
    particlesStr = map( particleToStr, allParticles )

    # Open the header file and write the list in a c++ vector
    f = open( fname, 'w' )
    f.write("# pdg, mass, mass upper, mass lower, width, width upper, width lower, I, G, C, name\n")
    f.write( "\n".join( particlesStr ) + "\n" )
    f.close()
    
    print ("Output written in '%s'"%( fname ))




