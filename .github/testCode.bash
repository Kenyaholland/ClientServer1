if grep -q socket src/main.cpp; then
    echo socket call found
else
    echo socket call not found
    exit 1
fi

if grep -q bind src/main.cpp; then
    echo bind call found
else
    echo bind call not found
    exit 1
fi

if grep -q listen src/main.cpp; then
    echo listen call found
else
    echo listen call not found
    exit 1
fi

if grep -q accept src/main.cpp; then
    echo accept call found
else
    echo accept call not found
    exit 1
fi

if grep -q recv src/main.cpp; then
    echo recv call found
else
    echo recv call not found
    exit 1
fi

if grep -q send src/main.cpp; then
    echo send call found
else
    echo send call not found
    exit 1
fi
