using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BallScript : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
     void Update()
    {
        var rigidBody = GetComponent<Rigidbody>();

        if(Input.GetKeyUp(KeyCode.LeftArrow)||Input.GetKeyUp(KeyCode.A))
        {
            rigidBody.AddForce(-100,0,0);
        }
        if(Input.GetKeyUp(KeyCode.RightArrow)||Input.GetKeyUp(KeyCode.D))
        {
            rigidBody.AddForce(+100,0,0);
        }
        
        if(Input.GetKeyUp(KeyCode.Space))
        {
            if(Input.GetKey(KeyCode.LeftShift))
            {
                if(Input.GetKey(KeyCode.LeftControl)){
                    transform.position = new Vector3(0,0.5f, 0);
                    transform.rotation = Quaternion.Euler(0,0,0);
                    rigidBody.velocity = Vector3.zero;
                    rigidBody.angularVelocity = Vector3.zero;
                }
                else{
                    transform.position = new Vector3(0,0.5f, 0);
                    transform.rotation = Quaternion.Euler(0,0,0);
                    rigidBody.velocity = Vector3.zero;
                    rigidBody.angularVelocity = Vector3.zero;
                }
                
            }
            else
            {
                rigidBody.AddForce(0,0,+100);
            }
        }
    }
}
