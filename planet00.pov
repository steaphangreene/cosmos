camera {
  orthographic
  location <1, 0, -20>
  look_at  <0, 0, 0>
  right 1*x
  angle 6.5
  }

object {
  sphere {
    <0.0,0.0,0.0>,
    0.8
    }  
  texture {
    pigment {
      color <0.537254,0.376470,0.098039>
      }
    }
  }  

light_source { <50.0, 00.0, -25.0> color <1.0,1.0,1.0>}

