using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PinScript : MonoBehaviour
{

    Vector3 startPosition;
    // Start is called before the first frame update
    
    void Start()
    {
        startPosition = transform.position;
    }

    // Update is called once per frame
    void Update()
    {
        var rigidBody = GetComponent<Rigidbody>();
        if (Input.GetKeyUp(KeyCode.Space))
        {
            if(Input.GetKey(KeyCode.LeftShift))
            {
                if(Input.GetKey(KeyCode.LeftControl)){
                    transform.position = startPosition;
                    transform.rotation = Quaternion.Euler(0, 0, 0);
                    rigidBody.velocity = Vector3.zero;
                    rigidBody.angularVelocity = Vector3.zero;
                }         
             
            }

        }
    }
}
