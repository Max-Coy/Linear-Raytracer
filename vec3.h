#ifndef VEC3_H
#define VEC3_H

class vec3{
    public:
        double e[3];

        vec3() : e{0,0,0} {}
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); } //adding negative vectors
        double operator[](int i) const { return e[i]; } // adding indexing
        double& operator[](int i) { return e[i]; } //no idea

        vec3& operator+=(const vec3& v){ //vector addition
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this; //voodoo
        }

        vec3& operator*=(double t){ //scalar multiplication
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        vec3& operator/=(double t) { //scalar division
            return *this *= 1/t;
        }

        double length_squared() const{
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        double length() const {
            return std::sqrt(length_squared());
        }

        static vec3 random(){
            return vec3(random_double(), random_double(), random_double());
        }

        static vec3 random(double min, double max){
            return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
        }

        bool near_zero() const{
            // returns true if length of vector is 'close' to 0 (~ < 1.7e-24)
            auto s = 1e-8;
            return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
        }


};


// Adding point3 to help differentiate colors and rays (same class)
using point3 = vec3;

// utility functions
inline std::ostream& operator<<(std::ostream& out, const vec3& v) { //tostring?
    return out << v.e[0] << " " << v.e[1] << " " << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v){ //inline vector addition
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v){ //inline vector subtraction
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v){
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3& v, double t){
    return t * v;
}

inline vec3 operator/(const vec3& v, double t){
    return (1/t) * v;
}

inline double dot(const vec3& u, const vec3& v){
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v){
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(const vec3& v){
    return v / v.length();
}

inline vec3 random_unit_vector(){
    double z = random_double(-1,1);
    double phi = random_double(0,two_pi);
    double r = sqrt(1 - z*z);

    return vec3(r*cos(phi), r*sin(phi), z);
}

inline vec3 random_on_hemisphere(const vec3& normal){
    vec3 on_unit_sphere = random_unit_vector();
    if(dot(on_unit_sphere,normal) > 0.0)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

inline vec3 reflect(const vec3& v, const vec3& n){
    return v - 2*dot(v,n)*n;
}

inline vec3 refract(const vec3&  uv, const vec3& n, double etai_over_etat){
    auto cos_theta = std::fmin(dot(-uv,n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

inline vec3 random_in_unit_disk(){
    // Concentric Disk Mapping
    double u1 = random_double(-1,1);
    double u2 = random_double(-1,1);
    double r, theta; 
    // Avoiding Division by 0
    if(u1 == 0.0 && u2 == 0.0)
        return vec3(0.0, 0.0, 0.0);

    if(std::abs(u1) > std::abs(u2)){
        r = u1;
        theta = pi_d_4 * (u2/u1);
    }else{
        r = u2;
        theta = pi_d_2 - pi_d_4 * (u1/u2);
    }
    return vec3(r * std::cos(theta), r * std::sin(theta), 0.0);
}

#endif