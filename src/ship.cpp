#include "ship.h"
#include "helper.h"

// Standard double definition
inline constexpr double PI = 3.14159265358979323846;

Ship::Ship() : vertices(sf::PrimitiveType::TriangleStrip, 4)  {
    // define the positions
    vertices[0].position = sf::Vector2f(-0.7071f, -0.7071f);
    vertices[1].position = sf::Vector2f(1.0f, 0.0f);;
    vertices[2].position = sf::Vector2f(0.0f, 0.0f);
    vertices[3].position = sf::Vector2f(-0.7071f, 0.7071f);

    // define the color of the ship. All vertices get the same color
    for (std::size_t i = 0; i < vertices.getVertexCount(); ++i) {
        vertices[i].color = sf::Color::Red;
    }
}

void Ship::draw(sf::RenderTarget& target, sf::RenderStates states) const {   
    target.draw( vertices, getTransform() );  
    
    // draw the projectiles
    for ( const auto& proj : projectiles ) {
        
        // We'll start with a simple circle shape to represent the projectile.
        sf::CircleShape shape(PROJECTILE_RADIUS, 12);
        
        // Set the origin to the center of the circle rather than the default top-left corner.
        shape.setOrigin( sf::Vector2f{PROJECTILE_RADIUS, PROJECTILE_RADIUS} );
        
        // set fill color - we'll use yellow for the projectiles but you can choose any color you like.
        shape.setFillColor(sf::Color::Yellow);
        
        // set position and rotation based on this projectile's current state. 

        // TO DO: Your code here
        // ...
        
        // scale the projectile to make it look elliptical like a bullet.

        // TO DO: Your code here
        // ...
        
        target.draw(shape);
    }    
}


Ship& Ship::operator+=( float dt ) {
        // update position and angle of the ship based on the current speed, acceleration, and angular speed.
        float rangle = angle * PI / 180.;
        pos += sf::Vector2f( u * cos(rangle ) * dt + 0.5f * a * cos(rangle) * dt * dt, 
                             u * sin(rangle ) * dt + 0.5f * a * sin(rangle) * dt * dt );
        
        // update angle keeping it in the range [0.0, 360.0)]
        angle += w*dt;
        angle = fmod(angle, 360.0f);
        if (angle < 0.0f) {
            angle += 360.0f;
        }
        
        // check for wrap-around of the ship's position
        wrapAround(pos);

        // update speed
        u += a * dt;

        // check if we need to stop accelerating
        if ( u > MAX_SPEED || u < 0.0f) {
            a = 0.0f;
            if( u > MAX_SPEED ) {
                u = MAX_SPEED;
            }
            if( u < 0.0f ) {
                u = 0.0f;
            }
        }

        // set the transformations of the ship in the following order of function calls: 
        // setPosition, setRotation, setScale.
        setPosition( pos );
        setRotation( sf::degrees( angle ) );
        setScale( sf::Vector2f{ SIZE_SCALE, SIZE_SCALE} );

        // update the projectiles
        
        // advance existing projectiles
        for ( auto& proj : projectiles ) {
            // TO DO: Your code here
            // ...
        }

        // clean up projectiles that are out of the screen.
        auto criterion = [](Projectile proj ) {
            return (abs(proj.pos.x) > 1.f) || (abs(proj.pos.y) > 1.f);
        };
        projectiles.erase( std::remove_if( projectiles.begin(), projectiles.end(), criterion), projectiles.end());
        
        // handle weapon - see if we need to fire a new projectile
        if (weapon_on) {
            if (ttnf <= 0.f) {
                // fire a projectile based on the current position and angle of the ship
                // TO DO: Your code here
                // ...
                
                // reset the time to next fire based on the fire rate
                ttnf = 1.0f / FIRE_RATE;
            }
            ttnf -= dt;
        }

 
        return *this;
    }


