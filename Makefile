CXXFLAGS+=--std=c++11 -g
LDFLAGS+=-pthread
# Add .d to Make's recognized suffixes.
SUFFIXES += .d .hpp

#We don't need to clean up when we're making these targets
NODEPS:=clean tags svn
#Find all the C++ files in the src/ directory
SOURCES:=$(shell find src/ -name "*.cpp")
#These are the dependency files, which make will clean up after it creates them
DEPFILES:=$(patsubst %.cpp,%.d,$(SOURCES))
OBJS:=$(patsubst src/%.cpp,obj/%.o,$(SOURCES))

#Don't create dependencies when we're cleaning, for instance
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
    #Chances are, these files don't exist.  GMake will create them and
    #clean up automatically afterwards
    -include $(DEPFILES)
endif

clean:
	rm -rf obj src/*.d
server: $(OBJS) Makefile
	gcc $(LDFLAGS) -o server $(OBJS) -lstdc++

#This is the rule for creating the dependency files
src/%.d: src/%.cpp
	$(CXX) $(CXXFLAGS) -MM -MT '$(patsubst src/%.cpp,obj/%.o,$<)' $< -MF $@
	echo $(OBJS)


#This rule does the compilation
obj/%.o: src/%.cpp src/%.d Makefile
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ -c $<
