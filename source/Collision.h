vec3 ClosestPtPointTriangle(vec3* P, vec3* A, vec3* B, vec3* C)
{
    A->x *= 100.0f;
    A->y *= 100.0f;
    A->z *= 100.0f;
    B->x *= 100.0f;
    B->y *= 100.0f;
    B->z *= 100.0f;
    C->x *= 100.0f;
    C->y *= 100.0f;
    C->z *= 100.0f;
    vec3 AB = Sub3P(B, A);
    vec3 AC = Sub3P(C, A);
    vec3 AP = Sub3P(P, A);

    f32 D1 = Dot3P(&AB, &AP);
    f32 D2 = Dot3P(&AC, &AP);
    if (D1 <= F0 && D2 <= F0)
        return *A;

    vec3 BP = Sub3P(P, B);
    f32 D3 = Dot3P(&AB, &BP);
    f32 D4 = Dot3P(&AC, &BP);
    if (D3 >= F0 && D4 <= D3)
        return *B;

    f32 Vc = D1 * D4 - D3 * D2;
    if (Vc <= F0 && D1 >= F0 && D3 <= F0)
    {
        f32 V = D1 / (D1 - D3);
        vec3 Vv = { V, V, V };
        return Add3(*A, Mul3P(&Vv, &AB));
    }

    vec3 CP = Sub3P(P, C);
    f32 D5 = Dot3P(&AB, &CP);
    f32 D6 = Dot3P(&AC, &CP);
    if (D6 >= F0 && D5 <= D6)
        return *C;

    f32 Vb = D5 * D2 - D1 * D6;
    if (Vb <= F0 && D2 >= F0 && D6 <= F0)
    {
        f32 W = D2 / (D2 - D6);
        vec3 Ww = { W, W, W };
        return Add3(*A, Mul3P(&Ww, &AC));
    }

    f32 Va = D3 * D6 - D5 * D4;
    if (Va <= F0 && (D4 - D3) >= F0 && (D5 - D6) >= F0)
    {
        f32 W = (D4 - D3) / ((D4 - D3) + (D5 - D6));
        vec3 Ww = { W, W, W };
        return Add3(*B, Mul3(Ww, Sub3P(C, B)));
    }

    f32 Denom = f32(F1) / (Va + Vb + Vc);
    f32 V = Vb * Denom;
    vec3 Vv = { V, V, V };
    f32 W = Vc * Denom;
    vec3 Ww = { W, W, W };
    
    return Add3(Add3(*A, Mul3P(&AB, &Vv)), Mul3P(&AC, &Ww));

/*
    f32 Snom = Dot3(Sub3P(P, A), AB);
    f32 Sdenom = Dot3(Sub3P(P, B), Sub3P(A, B));

    f32 Tnom = Dot3(Sub3P(P, A), AC);
    f32 Tdenom = Dot3(Sub3P(P, C), Sub3P(A, C));

    if (Snom <= F0 && Tnom <= F0)
        return *A;

    f32 Unom = Dot3(Sub3P(P, B), BC);
    f32 Udenom = Dot3(Sub3P(P, C), Sub3P(B, C));

    if (Sdenom <= F0 && Unom <= F0)
        return *B;
    if (Tdenom <= F0 && Udenom <= F0)
        return *C;

    vec3 N = Cross3(Sub3P(B, A), Sub3P(C, A));
    f32 Vc = Dot3(N, Cross3(Sub3P(A, P), Sub3P(B, P)));

    if (Vc <= F0 && Snom >= F0 && Sdenom >= F0)
    {
        f32 T = Snom / (Snom + Sdenom);
        vec3 TV = { T, T, T };
        return Add3(*A, Mul3P(&TV, &AB));
    }
    
    f32 Va = Dot3(N, Cross3(Sub3P(B, P), Sub3P(C, P)));

    if (Va <= F0 && Unom >= F0 && Udenom >= F0)
    {

    }

    return AB;
    */
}
/*
bool SphereVsTriangle(vec3* SpherePos, f32 SphereRadius, vec3* A, vec3* B, vec3* C, vec3* P)
{
    *P = ClosestPtPointTriangle(SpherePos, A, B, C);
    vec3 V = Sub3P(P, SpherePos);
    return Dot3P(&V, &V) <= SphereRadius * SphereRadius;
}
*/

bool IntersectRayTriangle(vec3* ray_origin,vec3* ray_direction,vec3* v0,vec3* v1,vec3* v2,f32* Dist)
{      
    
       const f32 EPSILON = 0.0000001f;
       vec3 e1 = Sub3P(v1, v0);
       vec3 e2 = Sub3P(v2, v0);  
       vec3 h = Cross3P(ray_direction,&e2); 
       f32 a = Dot3P(&e1,&h);
       if ((a == FN0) || (a == F0)) return false;
       //if (a > -f32(EPSILON) && a < EPSILON) return false;
       f32 f = f32(F1) / a;
       vec3 s = Sub3P(ray_origin, v0);
       f32 u = f * Dot3P(&s,&h);
       if ((u < F0) || (u > F1)) return false;
       vec3 q = Cross3P(&s,&e1);
       f32 v = f * Dot3P(ray_direction,&q);
       if ((v < F0) || ((v + u) > F1)) return false;
       f32 t = f * Dot3P(&e2,&q);
       // if (t <= EPSILON) return FALSE; // line intersection but no ray intersection
       if (t > EPSILON)
       {
            Dist->Fixed = t.Fixed;
            return true;
       }
       return false;
    //   if (t < F0) return false;
    //   Dist->Fixed = t.Fixed;
      // return true;
   
  /*
    *v1 = Sub3P(v1, v0);
    *v2 = Sub3P(v2, v0);  
    vec3 h = Cross3P(ray_direction,v2); 
    f32 a = Dot3P(v1,&h);
    if ((a == FN0) || (a == F0)) return false;
    // if (a > -EPSILON && a < EPSILON) return FALSE;
    f32 f = f32(F1) / a;
    vec3 s = Sub3P(ray_origin, v0);
    f32 u = f * Dot3P(&s,&h);
    if ((u < F0) || (u > F1)) return false;
    vec3 q = Cross3P(&s,v1);
    a = f * Dot3P(ray_direction,&q);
    if ((a < F0) || ((a + u) > F1)) return false;
    a = f * Dot3P(v2,&q);
    // if (t <= EPSILON) return FALSE; // line intersection but no ray intersection
    if (a < F0) return false;
    *Dist = a;
    return true; 
    */
   /*
    vec3 v0v1 = Sub3P(v1, v0);; 
    vec3 v0v2 = Sub3P(v2, v0);; 
    vec3 pvec = Cross3P(ray_direction,&v0v2); 
    f32 det = Dot3P(&v0v1, &pvec); 
    // ray and triangle are parallel if det is close to 0
    f32 kEpsilon = 0.0001;
    if (fabsf32(det) < kEpsilon) return false; 
    f32 invDet = f32(F1) / det; 
 
    vec3 tvec = Sub3P(ray_origin, v0); 
    f32 u = Dot3P(&tvec, &pvec) * invDet; 
    if (u < F0 || u > F1) return false; 
 
    vec3 qvec = Cross3P(&tvec, &v0v1); 
    f32 v = Dot3P(ray_direction, &qvec) * invDet; 
    if (v < F0 || u + v > F1) return false; 
 
    *Dist = Dot3P(&v0v2, &qvec) * invDet; 
     if (*Dist < F0) return false;
 
    return true; 
    */
/*
    const float EPSILON = 0.0000001f;
    vec3 e1 = Sub3P(&v1, &v0);
    vec3 e2 = Sub3P(&v2, &v0);  
    vec3 h = Cross3P(&ray_direction,&e2); 
    f32 a = Dot3P(&e1,&h);
    if (a <EPSILON)
    {

        return false;
    }

    vec3 tvec = Sub3P(&ray_origin,&A);
    f32 u = Dot3P(&tvec,&h);
    if (u < F0 || u > det)
    {

        return false;
    }
    vec3 qvec = Cross3P(&tvec,&e1);
    f32 v = Dot3P(&ray_direction,&qvec);
    if (v < F0 || v + u > det)
    {

        return false;
    }

    return true;
    */
}

bool SphereRayVsTriangle(vec3* SpherePos, f32 SphereRadius, vec3* RayDir, f32 TriScale, vec3* A, vec3* B, vec3* C, vec3* Normal, f32* Dist)
{
   // if (TriScale.Fixed != 0)
   // {
    A->x *= TriScale;
    A->y *= TriScale;
    A->z *= TriScale;
    B->x *= TriScale;
    B->y *= TriScale;
    B->z *= TriScale;
    C->x *= TriScale;
    C->y *= TriScale;
    C->z *= TriScale;
    //}

    //*Dist = F0;
    bool Hit = IntersectRayTriangle(SpherePos, RayDir, A, B, C, Dist);

    if (Hit && Dist->Fixed <= SphereRadius.Fixed)
    {
        vec3 a = Sub3P(B, A);
        vec3 b = Sub3P(C, A);
        *Normal = Cross3P(&a, &b);
        Normalize3P(Normal);
        return true;
    }        
//    *Dist = F0;

    return false;
}
