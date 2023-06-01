using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Transformation : MonoBehaviour
{
    // Start is called before the first frame update
    public Mesh mesh;
    public MeshFilter meshFilter;
    void Start()
    {
        meshFilter = GetComponent<MeshFilter>();
        mesh = meshFilter.mesh;

        Scale();
        Rotate();
        Shear();
        Transform();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void Scale()
    {
        var M = Matrix4x4.identity;
        M[0,0] = 0.5f;
        M[0,1] = 0f;
        M[0,2] = 0f;
        M[0,3] = 0f;
        
        M[1,0] = 0f;
        M[1,1] = 0.5f;
        M[1,2] = 0f;
        M[1,3] = 0f;
        
        M[2,0] = 0f;
        M[2,1] = 0f;
        M[2,2] = 0.5f;
        M[2,3] = 0f;
        
        M[3,0] = 0f;
        M[3,1] = 0f;
        M[3,2] = 0f;
        M[3,3] = 1f;
        
        TransformMesh(M);
    }

    void Rotate()
    {
        var M = Matrix4x4.identity;
        M[0, 0] = 0.70711f; // cos45�ơ� 0.70711
        M[0, 1] = -0.70711f; // -sin45�ơ� -0.70711
        M[0, 2] = 0f;
        M[0, 3] = 0f;

        M[1, 0] = 0.70711f; // sin45�ơ� 0.70711
        M[1, 1] = 0.70711f; // cos45�ơ� 0.7071
        M[1, 2] = 0f;
        M[1, 3] = 0f;

        M[2, 0] = 0f;
        M[2, 1] = 0f;
        M[2, 2] = 0.5f; 
        M[2, 3] = 0f;

        M[3, 0] = 0f;
        M[3, 1] = 0f;
        M[3, 2] = 0f;
        M[3, 3] = 1f;

        TransformMesh(M);
    }

    void Shear()
    {
        var M = Matrix4x4.identity;
        M[0, 0] = 1f; 
        M[0, 1] = 0f; 
        M[0, 2] = 0f;
        M[0, 3] = 0f;

        M[1, 0] = 0.5f; 
        M[1, 1] = 1f; 
        M[1, 2] = 0f;
        M[1, 3] = 0f;

        M[2, 0] = 0.5f;
        M[2, 1] = 0f;
        M[2, 2] = 1f; 
        M[2, 3] = 0f;

        M[3, 0] = 0f;
        M[3, 1] = 0f;
        M[3, 2] = 0f;
        M[3, 3] = 1f;

        TransformMesh(M);
    }

    void Transform()
    {
        var M = Matrix4x4.identity;
        M[0, 0] = 1f;
        M[0, 1] = 0f;
        M[0, 2] = 0f;
        M[0, 3] = 0f;

        M[1, 0] = 0f;
        M[1, 1] = 1f;
        M[1, 2] = 0f;
        M[1, 3] = 0f;

        M[2, 0] = 0f;
        M[2, 1] = 0f;
        M[2, 2] = 1f;
        M[2, 3] = 0f;

        M[3, 0] = 0.5f;
        M[3, 1] = 0.5f;
        M[3, 2] = 0.5f;
        M[3, 3] = 1f;

        TransformMesh(M);
    }

    void TransformMesh(Matrix4x4 T)
    {
        var srcVerts = mesh.vertices;
        var outVerts = new Vector3[srcVerts.Length];

        for(int i =0;i<outVerts.Length; ++i)
        {
            outVerts[i] = T.MultiplyPoint3x4(srcVerts[i]);
        }

        mesh.vertices = outVerts;
    }
}

