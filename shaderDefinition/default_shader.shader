{
    "name": "DefaultLit",
    "stages": [
        {
            "stage": "vertex",
            "path": "shader/gBuffer.vert"
        },
        {
            "stage": "fragment",
            "path": "shader/gBuffer.frag"
        }
    ],
    "parameters": [
        {
            "name": "albedoMap",
            "type": "texture2D",
            "set": 1,
            "binding": 1
        },
        {
            "name": "normalMap",
            "type": "texture2D",
            "set": 1,
            "binding": 2
        },
        {
            "name": "metallicRoughnessAOMap",
            "type": "texture2D",
            "set": 1,
            "binding": 3
        },
	 {
            "name": "metallicRoughnessAOMap",
            "type": "texture2D",
            "set": 1,
            "binding": 4
        }
    ]
}