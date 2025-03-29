function editOrSaveDetails(button) {
    const detailsContainer = button.closest('.details-container');
    const dataFields = detailsContainer.querySelectorAll('input, textarea');
    
    if (button.textContent === 'Edit') {
        dataFields.forEach(input => {
            input.removeAttribute('readonly');
            input.style.border = '2px solid #4CAF50';
            input.style.backgroundColor = '#ffffff';
        });
        button.textContent = 'Save';
    } else {
        dataFields.forEach(input => {
            input.style.border = '';  // Clear any previous border style
            input.style.backgroundColor = '';  // Clear background color
        });
        let isEmpty = false;
        dataFields.forEach(input => {
            if (!input.value.trim()) {
                isEmpty = true;
                input.style.border = '2px solid #ff0000';
            }
        });

        if (isEmpty) {
            alert('Please fill in all fields before saving.');
            return;
        }
       // Validation rules
       const details = {};
       dataFields.forEach(input => {
           if (input.name) {
               details[input.name] = input.value.trim();
           }
       });
       // Phone number validation
       const phoneRegex = /^[0-9]{10}$/;
       if (details.phone && !phoneRegex.test(details.phone)) {
           document.getElementById('phone').style.border = '2px solid #ff0000';
           alert("Invalid phone number");
           return;
       }
       // Digital ID validation
       const digitalidRegex = /^[0-9]{7}$/;
       if (details.digitalid && !digitalidRegex.test(details.digitalid)) {
           document.getElementById('digitalid').style.border = '2px solid #ff0000';
           alert("Invalid Digital ID");
           return;
       }
       // Registration number validation 
       const regnoRegex = /^[0-9]{13}$/;
       if (details.regno && !regnoRegex.test(details.regno)) {
           document.getElementById('regno').style.border = '2px solid #ff0000';
           alert("Invalid Registration Number");
           return;
       }
       // Name validation
       const nameRegex = /^[a-zA-Z][a-zA-Z\s\.]*$/;
       if (details.name && !nameRegex.test(details.name)) {
           document.getElementById('name').style.border = '2px solid #ff0000';
           alert("Invalid name");
           return;
       }
       // Email validation (must end with @domain.com)
       const emailRegex = /^[a-zA-Z0-9._-]+@[a-zA-Z]+\.com$/;
       if (details.email && !emailRegex.test(details.email)) {
           document.getElementById('email').style.border = '2px solid #ff0000';
           alert("Invalid email");
           return;
       }
       // Parent phone validation
       const parentphoneRegex = /^[0-9]{10}$/;
       if (details.parentphone && !parentphoneRegex.test(details.parentphone)) {
           document.getElementById('parentphone').style.border = '2px solid #ff0000';
           alert("Invalid parent phone number");
           return;
       }
       // Parent email validation
       const parentidRegex = /^[a-zA-Z0-9._-]+@[a-zA-Z]+\.com$/;
       if (details.parentid && !parentidRegex.test(details.parentid)) {
           document.getElementById('parentid').style.border = '2px solid #ff0000';
           alert("Invalid parent email");
           return;
       }
        dataFields.forEach(input => {
            input.setAttribute('readonly', true);
            input.style.border = '';
            input.style.backgroundColor = '#f9f9f9';
        });
        button.textContent = 'Edit';

        const confirmationMsg = document.createElement('div');
        confirmationMsg.textContent = 'Details saved successfully!';
        confirmationMsg.style.color = '#4CAF50';
        confirmationMsg.style.fontWeight = 'bold';
        confirmationMsg.style.marginTop = '10px';
        
        const existingMsg = detailsContainer.querySelector('.confirmation-msg');
        if (existingMsg) {
            existingMsg.remove();
        }
        
        confirmationMsg.classList.add('confirmation-msg');
        detailsContainer.appendChild(confirmationMsg);
        
        setTimeout(() => {
            confirmationMsg.remove();
        }, 3000);
        
    }
}

function editOrSaveTasks(button) {
    const taskContainer = button.closest('.task-container');
    const taskTitle = taskContainer.querySelector('.task-name');
    const taskDate = taskContainer.querySelector('.task-date');

    if (button.textContent === 'Edit') {
        taskTitle.removeAttribute('readonly');
        taskDate.removeAttribute('readonly');
        taskTitle.style.border = '2px solid #4CAF50';
        taskDate.style.border = '2px solid #4CAF50';
        taskTitle.style.backgroundColor = '#ffffff';
        taskDate.style.backgroundColor = '#ffffff';
        button.textContent = 'Save';
    } else {
        if (!taskTitle.value.trim()) {
            alert('Task Name cannot be empty.');
            taskTitle.focus();
            return;
        }
        
        if (!taskDate.value) {
            alert('Please select a valid date.');
            taskDate.focus();
            return;
        }

        taskTitle.setAttribute('readonly', true);
        taskDate.setAttribute('readonly', true);
        taskTitle.style.border = '';
        taskDate.style.border = '';
        taskTitle.style.backgroundColor = '#f9f9f9';
        taskDate.style.backgroundColor = '#f9f9f9';
        button.textContent = 'Edit';
        
        taskContainer.style.backgroundColor = '#e8f5e9';
        taskContainer.style.backgroundColor = '';
    }
}

function addTask(button) {
    const taskTitle = document.querySelector('#task-name').value.trim();
    const taskDate = document.querySelector('#task-date').value;

    if (!taskTitle) {
        alert('Task Name cannot be empty.');
        document.querySelector('#task-name').focus();
        return;
    }
    
    if (!taskDate) {
        alert('Please select a valid date.');
        document.querySelector('#task-date').focus();
        return;
    }

    const newTask = document.createElement('div');
    newTask.classList.add('task-container');
    newTask.innerHTML = `
        <input type="text" class="input-box task-name" value="${taskTitle}" readonly>
        <input type="date" class="input-date task-date" value="${taskDate}" readonly>
        <button class="edit-btn edit-task mentor-only-btn">Edit</button>
        <button class="delete-task mentor-only-btn">X</button>
    `;

    document.querySelector('.progress-container').appendChild(newTask);
    
    newTask.querySelector('.edit-task').addEventListener('click', function() {
        editOrSaveTasks(this);
    });
    
    newTask.querySelector('.delete-task').addEventListener('click', function() {
        deleteTask(this);
    });
    
    document.querySelector('#task-name').value = '';
    document.querySelector('#task-date').value = '';
    
    newTask.style.opacity = '1';
}

function deleteTask(button) {
    const taskContainer = button.closest('.task-container'); 
    if (confirm('Are you sure you want to delete this task?')) {
        taskContainer.remove();
    }
}

function editOrSaveMeeting(button) {
    const meetingCard = button.closest('.meeting-card');
    const noteContent = meetingCard.querySelector('p'); 
    const meetingDate = meetingCard.querySelector('.meeting-date');
    const editIcon = button.querySelector('img');      

    if (editIcon.getAttribute('alt') === 'Edit') {
        noteContent.contentEditable = 'true';    
        noteContent.focus();            
        noteContent.style.border = '2px solid #4CAF50'; 
        noteContent.style.padding = '8px';
        noteContent.style.borderRadius = '5px';
        noteContent.style.backgroundColor = '#ffffff';

        meetingDate.removeAttribute('readonly');
        meetingDate.style.border = '2px solid #4CAF50';  
        meetingDate.style.backgroundColor = '#ffffff';
                           
        editIcon.setAttribute('src', 'images/save_icon.png');
        editIcon.setAttribute('alt', 'Save');
    } else {
        const updatedText = noteContent.textContent.trim();
        const updatedDate = meetingDate.value;

        if (!updatedText) {
            alert('Note content cannot be empty.');
            noteContent.focus();
            return; 
        }
        
        if (!updatedDate) {
            alert('Please select a valid date.');
            meetingDate.focus();
            return;
        }

        noteContent.contentEditable = 'false';        
        noteContent.style.border = 'none';      
        noteContent.style.padding = '0';
        noteContent.style.backgroundColor = 'transparent';
        
        meetingDate.setAttribute('readonly', true);
        meetingDate.style.border = 'none';
        meetingDate.style.backgroundColor = 'transparent';

        editIcon.setAttribute('src', 'images/pencil_edit.png');
        editIcon.setAttribute('alt', 'Edit');
        
        // Add animation for saved changes
        meetingCard.style.backgroundColor = '#e8f5e9';
        setTimeout(() => {
            meetingCard.style.backgroundColor = '';
        }, 500);
    }
}

function addMeeting() {
    const meetingDate = document.querySelector('#meeting-date').value;
    const meetingSummary = document.querySelector('#meeting-summary').value.trim();

    if (!meetingSummary) {
        alert('Note content cannot be empty.');
        document.querySelector('#meeting-summary').focus();
        return; 
    }
    
    if (!meetingDate) {
        alert('Please select a valid date.');
        document.querySelector('#meeting-date').focus();
        return;
    }

    const newMeetingCard = document.createElement('div');
    newMeetingCard.classList.add('meeting-card');
    newMeetingCard.innerHTML = `
        <div class="meeting-header">
            <input type="date" class="meeting-date" value="${meetingDate}" readonly>
            <button class="edit-note mentor-only-btn">
                <img src="images/pencil_edit.png" alt="Edit">
            </button>
        </div>
        <div class="meeting-content">
            <p>${meetingSummary}</p>
            <button class="delete-meeting mentor-only-btn">🗑️</button>
        </div>
    `;

    const meetingHistory = document.querySelector('.meeting-history');
    meetingHistory.insertBefore(newMeetingCard, meetingHistory.firstChild);

    newMeetingCard.querySelector('.edit-note').addEventListener('click', function() {
        editOrSaveMeeting(this);
    });

    newMeetingCard.querySelector('.delete-meeting').addEventListener('click', function() {
        deleteMeeting(this);
    });

    document.querySelector('#meeting-date').value = '';
    document.querySelector('#meeting-summary').value = '';
    
    
}

function deleteMeeting(button) {
    const meetingCard = button.closest('.meeting-card'); 
    if (confirm('Are you sure you want to delete this meeting?')) {
        meetingCard.remove();
    }
}
window.onload = function() {
    const url_params = new URLSearchParams(window.location.search);
    const role = url_params.get('role'); /* retrieving the type of user (mentor/mentee) from url*/
    
    if (document.body.id === 'login-page') {
        document.getElementById('role').value = role;
        document.getElementById('login-form').addEventListener('submit', function(event) {
            event.preventDefault();
            if (role) {
                sessionStorage.setItem('role', role);
                if (role === 'mentor') {
                    window.location.href = 'mentor_menteeslist.html';
                } else if (role === 'mentee') {
                    window.location.href = 'main.html';
                }
            } else {
                alert('No role detected. Please select your role');
            }
        });
    }

    if (document.body.id == "menteeslist-page") {
        const mentee_cards = document.querySelectorAll('.mentee-card');

        mentee_cards.forEach(card => {
            card.addEventListener('click', function() {
                const chosenMentee = card.getAttribute('data-mentee');
                sessionStorage.setItem('chosen_mentee', chosenMentee);
            });
        });
    }

    /* hiding buttons and elements based on the type of user */
    if (document.body.id === 'main-page') {
        const storedRole = sessionStorage.getItem('role');

        if (storedRole == 'mentor') {
            document.querySelectorAll('.mentee-only-btn').forEach(btn => btn.style.display = 'none'); /* hiding the class of mentee-only-btn */
            const sidebar_mentee_name = document.getElementById('mentee-name');
            sidebar_mentee_name.textContent = sessionStorage.getItem('chosen_mentee') || 'MENTEE 1';

            /* Meeting History edit functionalising */
            document.querySelectorAll('.edit-note').forEach(button => {
                button.addEventListener('click', function() {
                    editOrSaveMeeting(this);
                });
            });

            document.querySelector('#notes .save-btn').addEventListener('click', addMeeting);
            
            document.querySelectorAll('.delete-meeting').forEach(button => {
                button.addEventListener('click', function() {
                    deleteMeeting(this);
                });
            });
            
            document.querySelectorAll('.add-task').forEach(button => {
                button.addEventListener('click', function() {
                    addTask(this);
                });
            });

            document.querySelectorAll('.edit-task').forEach(button => {
                button.addEventListener('click', function() {
                    editOrSaveTasks(this);
                });
            });
            
            document.querySelectorAll('.delete-task').forEach(button => {
                button.addEventListener('click', function() {
                    deleteTask(this);
                });
            });
        } else if (storedRole == 'mentee') {
            document.querySelectorAll('.mentor-only-btn, .mentor-only-container').forEach(elem => elem.style.display = 'none'); /* hiding the class of mentor-only-btn, mentor-only container */
            
            const sidebar_mentee_name = document.getElementById('mentee-name'); 
            sidebar_mentee_name.style.backgroundColor = '#4CAF50'; 
            sidebar_mentee_name.textContent = 'MY DASHBOARD';

            const header_role = document.getElementById('user-role');
            header_role.textContent = 'MENTEE';
            
            const headerLink = document.querySelector('a[href="mentor_menteeslist.html?role=mentor"]');
            if (headerLink) {
                headerLink.setAttribute('href', 'index.html');
            }

            document.querySelectorAll('.edit-details-btn').forEach(button => {
                button.addEventListener('click', function() {
                    editOrSaveDetails(this);
                });
            });
        }
        
        if (!window.location.hash) {
            window.location.hash = 'details';
        }
        
        const currentHash = window.location.hash || '#details';
        document.querySelector(`.sidebar a[href="${currentHash}"]`)?.classList.add('active');
        
        const contentId = currentHash.substring(1);
        document.getElementById(contentId)?.classList.add('active-section');
    
        window.addEventListener('hashchange', function() {
            const hash = window.location.hash || '#details';
            
            document.querySelectorAll('.sidebar a').forEach(link => {
                link.classList.remove('active');
            });
            
            document.querySelector(`.sidebar a[href="${hash}"]`)?.classList.add('active');
            
            document.querySelectorAll('.content').forEach(section => {
                section.classList.remove('active-section');
            });
            
            const id = hash.substring(1);
            document.getElementById(id)?.classList.add('active-section');
        });
    }
};