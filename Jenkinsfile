#!groovy
node('worker') {
  try {
    stage('Checkout') {
      checkout([
        $class: 'GitSCM',
        branches: scm.branches,
        extensions: scm.extensions + [[$class: 'CleanBeforeCheckout']],
        userRemoteConfigs: scm.userRemoteConfigs
      ])
    }
    stage('Build') {
      sh 'mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release && make'
      echo 'Build Complete!'
    }
    stage('Unit Tests') {
      sh 'mkdir build_tests && cd build_tests && cmake .. -DCMAKE_BUILD_TYPE=Release && make run-test-steps'
      echo 'Tests Complete!'
    }
  }
  catch(Exception e) {
    throw e;
  }
  finally {
    deleteDir()
  }
}
