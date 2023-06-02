using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Rotation : MonoBehaviour
{
    // Start is called before the first frame update
    public float obital_speed;
    public float selrot_speed;
    private Transform parent;
    void Start()
    {
        obital_speed = Random.Range(1.0f, 50.0f);
        selrot_speed = Random.Range(1.0f, 50.0f);
        parent = transform.parent;
    }

    // Update is called once per frame
    void Update()
    {
        transform.Rotate(Vector3.up, selrot_speed * Time.deltaTime);
        if(parent) {
            transform.RotateAround(parent.transform.position, Vector3.up, obital_speed * Time.deltaTime);
        }
    }
}
