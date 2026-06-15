{
    "name": "DefaultLit",
    "stages": [
        {
            "stage": "vertex",
            "path": "shader/bin/skyVert.spv"
        },
        {
            "stage": "fragment",
            "path": "shader/bin/skyFrag.spv"
        }
    ],
    "parameters": [
        {
            "name": "albedoMap",
            "type": "texture2D",
            "set": 1,
            "binding": 1
        }
    ]
}