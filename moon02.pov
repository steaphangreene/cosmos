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
    0.2
    }  
  texture {
    pigment {
      color <0.5,0.5,0.5>
      }
    }
  }  

light_source { <50.0, 00.0, -25.0> color <1.0,1.0,1.0>}

